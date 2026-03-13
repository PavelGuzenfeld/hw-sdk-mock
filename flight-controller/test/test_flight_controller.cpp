#include <doctest/doctest.h>
#include "flight-controller/flight_controller.hpp"

using hw_sdk_mock::FlightController;
using RC = FlightController::ResponseCode;
using State = FlightController::State;

// Helper: retry until SUCCESS (random ~5% failure rate)
static RC retryUntilSuccess(auto &&fn)
{
    for (int i = 0; i < 100; ++i)
    {
        auto rc = fn();
        if (rc == RC::SUCCESS)
            return rc;
        CHECK((rc == RC::CONNECTION_ERROR || rc == RC::HARDWARE_ERROR));
    }
    FAIL("failed after 100 retries — statistically impossible");
    return RC::HARDWARE_ERROR;
}

TEST_CASE("initial state is DISARMED")
{
    FlightController fc;
    CHECK(fc.getState() == State::DISARMED);
}

TEST_CASE("happy path: arm -> takeOff -> goTo -> land -> disarm")
{
    FlightController fc;

    retryUntilSuccess([&] { return fc.arm(); });
    CHECK(fc.getState() == State::ARMED);

    retryUntilSuccess([&] { return fc.takeOff(50.0); });
    CHECK(fc.getState() == State::AIRBORNE);

    retryUntilSuccess([&] { return fc.goTo(32.0, 34.8, 100.0); });
    CHECK(fc.getState() == State::AIRBORNE);

    retryUntilSuccess([&] { return fc.land(); });
    CHECK(fc.getState() == State::ARMED);

    retryUntilSuccess([&] { return fc.disarm(); });
    CHECK(fc.getState() == State::DISARMED);
}

TEST_CASE("arm rejects when already armed")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });

    CHECK(fc.arm() == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::ARMED);
}

TEST_CASE("arm rejects when airborne")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });

    CHECK(fc.arm() == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::AIRBORNE);
}

TEST_CASE("disarm rejects when already disarmed")
{
    FlightController fc;
    CHECK(fc.disarm() == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::DISARMED);
}

TEST_CASE("disarm rejects when airborne")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });

    CHECK(fc.disarm() == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::AIRBORNE);
}

TEST_CASE("takeOff rejects when disarmed")
{
    FlightController fc;
    CHECK(fc.takeOff(10.0) == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::DISARMED);
}

TEST_CASE("takeOff rejects when already airborne")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });

    CHECK(fc.takeOff(20.0) == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::AIRBORNE);
}

TEST_CASE("takeOff rejects invalid altitude")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });

    CHECK(fc.takeOff(0.0) == RC::INVALID_COMMAND);
    CHECK(fc.takeOff(-5.0) == RC::INVALID_COMMAND);
    CHECK(fc.takeOff(10001.0) == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::ARMED);
}

TEST_CASE("land rejects when not airborne")
{
    FlightController fc;
    CHECK(fc.land() == RC::INVALID_COMMAND);

    retryUntilSuccess([&] { return fc.arm(); });
    CHECK(fc.land() == RC::INVALID_COMMAND);
    CHECK(fc.getState() == State::ARMED);
}

TEST_CASE("land transitions to ARMED, not DISARMED")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });
    retryUntilSuccess([&] { return fc.land(); });

    CHECK(fc.getState() == State::ARMED);
}

TEST_CASE("goTo rejects when not airborne")
{
    FlightController fc;
    CHECK(fc.goTo(0.0, 0.0, 100.0) == RC::INVALID_COMMAND);

    retryUntilSuccess([&] { return fc.arm(); });
    CHECK(fc.goTo(0.0, 0.0, 100.0) == RC::INVALID_COMMAND);
}

TEST_CASE("goTo rejects invalid coordinates")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });

    CHECK(fc.goTo(-91.0, 0.0, 100.0) == RC::INVALID_COMMAND);
    CHECK(fc.goTo(91.0, 0.0, 100.0) == RC::INVALID_COMMAND);
    CHECK(fc.goTo(0.0, -181.0, 100.0) == RC::INVALID_COMMAND);
    CHECK(fc.goTo(0.0, 181.0, 100.0) == RC::INVALID_COMMAND);
    CHECK(fc.goTo(0.0, 0.0, 0.0) == RC::INVALID_COMMAND);
    CHECK(fc.goTo(0.0, 0.0, 10001.0) == RC::INVALID_COMMAND);
}

TEST_CASE("goTo accepts boundary coordinates")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });

    retryUntilSuccess([&] { return fc.goTo(-90.0, -180.0, 0.1); });
    retryUntilSuccess([&] { return fc.goTo(90.0, 180.0, 10000.0); });
    CHECK(fc.getState() == State::AIRBORNE);
}

TEST_CASE("goHome rejects when not airborne")
{
    FlightController fc;
    CHECK(fc.goHome() == RC::INVALID_COMMAND);

    retryUntilSuccess([&] { return fc.arm(); });
    CHECK(fc.goHome() == RC::INVALID_COMMAND);
}

TEST_CASE("goHome transitions to ARMED")
{
    FlightController fc;
    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(10.0); });
    retryUntilSuccess([&] { return fc.goHome(); });

    CHECK(fc.getState() == State::ARMED);
}

TEST_CASE("hw failure does not change state")
{
    // Run many attempts — on HW error, state should stay unchanged
    for (int trial = 0; trial < 200; ++trial)
    {
        FlightController fc;
        auto rc = fc.arm();
        if (rc != RC::SUCCESS)
        {
            CHECK(fc.getState() == State::DISARMED);
        }
    }
}

TEST_CASE("full mission cycle")
{
    FlightController fc;

    retryUntilSuccess([&] { return fc.arm(); });
    retryUntilSuccess([&] { return fc.takeOff(50.0); });
    retryUntilSuccess([&] { return fc.goTo(32.0, 34.8, 100.0); });
    retryUntilSuccess([&] { return fc.goTo(32.1, 34.9, 200.0); });
    retryUntilSuccess([&] { return fc.goTo(32.2, 35.0, 150.0); });
    retryUntilSuccess([&] { return fc.land(); });
    retryUntilSuccess([&] { return fc.disarm(); });

    CHECK(fc.getState() == State::DISARMED);

    // Can re-arm after full cycle
    retryUntilSuccess([&] { return fc.arm(); });
    CHECK(fc.getState() == State::ARMED);
}
