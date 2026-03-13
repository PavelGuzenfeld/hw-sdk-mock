#include "flight-controller/flight_controller.hpp"

#include <random>
#include <string_view>

#include <fmt/format.h>

namespace hw_sdk_mock
{
    namespace
    {
        // ~5% chance of hardware/connection failure
        bool randomFailure()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<int> dis(1, 20);
            return dis(gen) == 1;
        }

        FlightController::ResponseCode randomHwError()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<int> dis(0, 1);
            return dis(gen) == 0 ? FlightController::ResponseCode::CONNECTION_ERROR
                                 : FlightController::ResponseCode::HARDWARE_ERROR;
        }

        std::string_view responseCodeToString(FlightController::ResponseCode code)
        {
            switch (code)
            {
            case FlightController::ResponseCode::SUCCESS:
                return "SUCCESS";
            case FlightController::ResponseCode::CONNECTION_ERROR:
                return "CONNECTION_ERROR";
            case FlightController::ResponseCode::HARDWARE_ERROR:
                return "HARDWARE_ERROR";
            case FlightController::ResponseCode::INVALID_COMMAND:
                return "INVALID_COMMAND";
            default:
                return "UNKNOWN_RESPONSE";
            }
        }

        std::string_view stateToString(FlightController::State state)
        {
            switch (state)
            {
            case FlightController::State::DISARMED:
                return "DISARMED";
            case FlightController::State::ARMED:
                return "ARMED";
            case FlightController::State::AIRBORNE:
                return "AIRBORNE";
            default:
                return "UNKNOWN_STATE";
            }
        }

        void printResult(std::string_view command, FlightController::ResponseCode response,
                         FlightController::State state)
        {
            fmt::print("Executing {}: {} [state: {}]\n", command, responseCodeToString(response),
                       stateToString(state));
        }
    }

    FlightController::ResponseCode FlightController::arm()
    {
        if (state_ != State::DISARMED)
        {
            printResult("ARM", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            printResult("ARM", err, state_);
            return err;
        }
        state_ = State::ARMED;
        printResult("ARM", ResponseCode::SUCCESS, state_);
        return ResponseCode::SUCCESS;
    }

    FlightController::ResponseCode FlightController::disarm()
    {
        if (state_ == State::DISARMED)
        {
            printResult("DISARM", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (state_ == State::AIRBORNE)
        {
            printResult("DISARM", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            printResult("DISARM", err, state_);
            return err;
        }
        state_ = State::DISARMED;
        printResult("DISARM", ResponseCode::SUCCESS, state_);
        return ResponseCode::SUCCESS;
    }

    FlightController::ResponseCode FlightController::takeOff(double altitude)
    {
        if (state_ != State::ARMED)
        {
            printResult("TAKEOFF", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (altitude <= 0.0 || altitude > 10000.0)
        {
            printResult("TAKEOFF", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            printResult("TAKEOFF", err, state_);
            return err;
        }
        state_ = State::AIRBORNE;
        fmt::print("Executing TAKEOFF (alt: {}): SUCCESS [state: {}]\n",
                   altitude, stateToString(state_));
        return ResponseCode::SUCCESS;
    }

    FlightController::ResponseCode FlightController::land()
    {
        if (state_ != State::AIRBORNE)
        {
            printResult("LAND", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            printResult("LAND", err, state_);
            return err;
        }
        state_ = State::ARMED;
        printResult("LAND", ResponseCode::SUCCESS, state_);
        return ResponseCode::SUCCESS;
    }

    FlightController::ResponseCode FlightController::goHome()
    {
        if (state_ != State::AIRBORNE)
        {
            printResult("GO_HOME", ResponseCode::INVALID_COMMAND, state_);
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            printResult("GO_HOME", err, state_);
            return err;
        }
        state_ = State::ARMED;
        printResult("GO_HOME", ResponseCode::SUCCESS, state_);
        return ResponseCode::SUCCESS;
    }

    FlightController::ResponseCode FlightController::goTo(double latitude, double longitude, double altitude)
    {
        if (state_ != State::AIRBORNE)
        {
            fmt::print("Executing GOTO (lat: {}, lon: {}, alt: {}): INVALID_COMMAND [state: {}]\n",
                       latitude, longitude, altitude, stateToString(state_));
            return ResponseCode::INVALID_COMMAND;
        }
        if (latitude < -90.0 || latitude > 90.0 ||
            longitude < -180.0 || longitude > 180.0 ||
            altitude <= 0.0 || altitude > 10000.0)
        {
            fmt::print("Executing GOTO (lat: {}, lon: {}, alt: {}): INVALID_COMMAND [state: {}]\n",
                       latitude, longitude, altitude, stateToString(state_));
            return ResponseCode::INVALID_COMMAND;
        }
        if (randomFailure())
        {
            auto err = randomHwError();
            fmt::print("Executing GOTO (lat: {}, lon: {}, alt: {}): {} [state: {}]\n",
                       latitude, longitude, altitude, responseCodeToString(err), stateToString(state_));
            return err;
        }
        fmt::print("Executing GOTO (lat: {}, lon: {}, alt: {}): SUCCESS [state: {}]\n",
                   latitude, longitude, altitude, stateToString(state_));
        return ResponseCode::SUCCESS;
    }

    FlightController::State FlightController::getState() const
    {
        return state_;
    }

} // namespace hw_sdk_mock
