#pragma once

#include <optional>

namespace hw_sdk_mock
{
    class Gps
    {
    public:
        struct Location
        {
            double latitude;  // Latitude in degrees (-90 to 90)
            double longitude; // Longitude in degrees (-180 to 180)
            double altitude;  // Altitude in meters (0 to 10000)
        };

        enum class SignalQuality
        {
            NO_SIGNAL = 0,
            POOR,
            FAIR,
            GOOD,
            EXCELLENT
        };

        Gps() = default;
        virtual ~Gps() = default;
        Gps(const Gps &) = delete;
        Gps &operator=(const Gps &) = delete;
        Gps(Gps &&) = default;
        Gps &operator=(Gps &&) = default;

        /**
         * @brief Get the current GPS location
         * @return Location if signal is available, std::nullopt if NO_SIGNAL
         */
        std::optional<Location> getLocation();
        SignalQuality getSignalQuality();

    private:
        Location position_{32.0, 34.8, 100.0}; // Initial position (Tel Aviv area)
        SignalQuality quality_ = SignalQuality::GOOD;
    };
} // namespace hw_sdk_mock
