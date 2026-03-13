#include "gps/gps.hpp"

#include <algorithm>
#include <random>

#include <fmt/format.h>

namespace hw_sdk_mock
{

    namespace
    {
        std::mt19937 &rng()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            return gen;
        }

        double drift(double stddev)
        {
            std::normal_distribution<double> dis(0.0, stddev);
            return dis(rng());
        }

        // Signal quality tends to stay similar between reads, with occasional shifts
        Gps::SignalQuality evolveSignalQuality(Gps::SignalQuality current)
        {
            // 80% chance to stay the same, 10% up, 10% down
            std::uniform_int_distribution<int> dis(1, 10);
            int roll = dis(rng());

            int val = static_cast<int>(current);
            if (roll == 1)
                val = std::max(0, val - 1);
            else if (roll == 2)
                val = std::min(4, val + 1);

            return static_cast<Gps::SignalQuality>(val);
        }

        // Position error (stddev in degrees) based on signal quality
        // EXCELLENT: ~1m, GOOD: ~5m, FAIR: ~20m, POOR: ~100m
        double positionNoiseForQuality(Gps::SignalQuality quality)
        {
            switch (quality)
            {
            case Gps::SignalQuality::EXCELLENT:
                return 0.00001;  // ~1m
            case Gps::SignalQuality::GOOD:
                return 0.00005;  // ~5m
            case Gps::SignalQuality::FAIR:
                return 0.0002;   // ~20m
            case Gps::SignalQuality::POOR:
                return 0.001;    // ~100m
            default:
                return 0.0;
            }
        }

        std::string_view signalQualityToString(Gps::SignalQuality quality)
        {
            switch (quality)
            {
            case Gps::SignalQuality::NO_SIGNAL:
                return "NO_SIGNAL";
            case Gps::SignalQuality::POOR:
                return "POOR";
            case Gps::SignalQuality::FAIR:
                return "FAIR";
            case Gps::SignalQuality::GOOD:
                return "GOOD";
            case Gps::SignalQuality::EXCELLENT:
                return "EXCELLENT";
            default:
                return "UNKNOWN_SIGNAL_QUALITY";
            }
        }
    }

    std::optional<Gps::Location> Gps::getLocation()
    {
        quality_ = evolveSignalQuality(quality_);

        if (quality_ == SignalQuality::NO_SIGNAL)
        {
            fmt::print("GPS Location - NO_SIGNAL\n");
            return std::nullopt;
        }

        double noise = positionNoiseForQuality(quality_);

        position_.latitude = std::clamp(position_.latitude + drift(noise), -90.0, 90.0);
        position_.longitude = std::clamp(position_.longitude + drift(noise), -180.0, 180.0);
        position_.altitude = std::clamp(position_.altitude + drift(noise * 10000.0), 0.0, 10000.0);

        fmt::print("GPS Location - Latitude: {:.6f}, Longitude: {:.6f}, Altitude: {:.6f} meters [signal: {}]\n",
                   position_.latitude, position_.longitude, position_.altitude,
                   signalQualityToString(quality_));

        return position_;
    }

    Gps::SignalQuality Gps::getSignalQuality()
    {
        quality_ = evolveSignalQuality(quality_);
        fmt::print("GPS Signal Quality: {}\n", signalQualityToString(quality_));
        return quality_;
    }

} // namespace hw_sdk_mock
