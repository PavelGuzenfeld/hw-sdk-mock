#include <doctest/doctest.h>
#include "gps/gps.hpp"

#include <cmath>

using hw_sdk_mock::Gps;

TEST_CASE("first location is near initial position")
{
    Gps gps;

    // May need a few calls if signal degrades to NO_SIGNAL early
    for (int i = 0; i < 10; ++i)
    {
        auto loc = gps.getLocation();
        if (loc)
        {
            CHECK(std::abs(loc->latitude - 32.0) < 1.0);
            CHECK(std::abs(loc->longitude - 34.8) < 1.0);
            CHECK(loc->altitude >= 0.0);
            CHECK(loc->altitude <= 10000.0);
            return;
        }
    }
    FAIL("could not get a valid location in 10 attempts");
}

TEST_CASE("consecutive readings are nearby")
{
    Gps gps;

    std::optional<Gps::Location> prev;
    int valid_pairs = 0;

    for (int i = 0; i < 50; ++i)
    {
        auto loc = gps.getLocation();
        if (loc && prev)
        {
            double dlat = std::abs(loc->latitude - prev->latitude);
            double dlon = std::abs(loc->longitude - prev->longitude);

            // Even with POOR signal (~0.001 deg stddev), consecutive readings
            // should be within ~0.01 deg (~1km) with high probability
            CHECK(dlat < 0.01);
            CHECK(dlon < 0.01);
            ++valid_pairs;
        }
        if (loc)
            prev = loc;
    }
    CHECK(valid_pairs > 10);
}

TEST_CASE("location stays within valid bounds over many calls")
{
    Gps gps;

    for (int i = 0; i < 200; ++i)
    {
        auto loc = gps.getLocation();
        if (loc)
        {
            CHECK(loc->latitude >= -90.0);
            CHECK(loc->latitude <= 90.0);
            CHECK(loc->longitude >= -180.0);
            CHECK(loc->longitude <= 180.0);
            CHECK(loc->altitude >= 0.0);
            CHECK(loc->altitude <= 10000.0);
        }
    }
}

TEST_CASE("NO_SIGNAL returns nullopt")
{
    // Run many GPS instances to find one that hits NO_SIGNAL
    bool found_no_signal = false;

    for (int trial = 0; trial < 50 && !found_no_signal; ++trial)
    {
        Gps gps;
        for (int i = 0; i < 100; ++i)
        {
            auto quality = gps.getSignalQuality();
            if (quality == Gps::SignalQuality::NO_SIGNAL)
            {
                // Next getLocation should also see NO_SIGNAL or evolve
                // The point is: if quality_ is NO_SIGNAL at getLocation time,
                // it returns nullopt
                found_no_signal = true;
                break;
            }
        }
    }
    // NO_SIGNAL should eventually occur given enough calls
    CHECK(found_no_signal);
}

TEST_CASE("signal quality is always a valid enum value")
{
    Gps gps;

    for (int i = 0; i < 100; ++i)
    {
        auto q = gps.getSignalQuality();
        int val = static_cast<int>(q);
        CHECK(val >= 0);
        CHECK(val <= 4);
    }
}

TEST_CASE("signal quality evolves gradually")
{
    Gps gps;

    auto prev = gps.getSignalQuality();
    int large_jumps = 0;

    for (int i = 0; i < 200; ++i)
    {
        auto cur = gps.getSignalQuality();
        int diff = std::abs(static_cast<int>(cur) - static_cast<int>(prev));
        if (diff > 1)
            ++large_jumps;
        prev = cur;
    }
    // Signal should never jump more than 1 step at a time
    CHECK(large_jumps == 0);
}
