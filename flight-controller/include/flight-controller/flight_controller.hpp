#pragma once

namespace hw_sdk_mock
{
    class FlightController
    {
    public:
        enum class ResponseCode
        {
            SUCCESS = 0,
            CONNECTION_ERROR,
            HARDWARE_ERROR,
            INVALID_COMMAND
        };

        enum class State
        {
            DISARMED = 0,
            ARMED,
            AIRBORNE
        };

        FlightController() = default;
        virtual ~FlightController() = default;
        FlightController(const FlightController &) = delete;
        FlightController &operator=(const FlightController &) = delete;
        FlightController(FlightController &&) = default;
        FlightController &operator=(FlightController &&) = default;

        ResponseCode arm();
        ResponseCode disarm();

        /**
         * @brief Take off the drone
         * @param altitude Altitude in meters (0 to 10000)
         */
        ResponseCode takeOff(double altitude);
        ResponseCode land();
        ResponseCode goHome();

        /**
         * @brief Go to a specified location
         * @param latitude Latitude in degrees (-90 to 90)
         * @param longitude Longitude in degrees (-180 to 180)
         * @param altitude Altitude in meters (0 to 10000)
         */
        ResponseCode goTo(double latitude, double longitude, double altitude);

        State getState() const;

    private:
        State state_ = State::DISARMED;
    };
} // namespace hw_sdk_mock
