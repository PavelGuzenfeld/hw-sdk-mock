#include "gps/gps.hpp"

#include <iomanip>
#include <iostream>
#include <random>

namespace hw_sdk_mock
{

    namespace
    {
        double getRandomDouble(double min, double max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(min, max);
            return dis(gen);
        }

        Gps::SignalQuality getRandomSignalQuality()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 4);
            return static_cast<Gps::SignalQuality>(dis(gen));
        }

        std::string signalQualityToString(Gps::SignalQuality quality)
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

    Gps::Location Gps::getLocation()
    {
        Location location{
            getRandomDouble(-90.0, 90.0),   // Latitude range
            getRandomDouble(-180.0, 180.0), // Longitude range
            getRandomDouble(0.0, 10000.0)   // Altitude range in meters
        };

        std::cout << std::fixed << std::setprecision(6)
                  << "GPS Location - Latitude: " << location.latitude
                  << ", Longitude: " << location.longitude
                  << ", Altitude: " << location.altitude << " meters\n";

        return location;
    }

    Gps::SignalQuality Gps::getSignalQuality()
    {
        SignalQuality quality = getRandomSignalQuality();
        std::cout << "GPS Signal Quality: " << signalQualityToString(quality) << "\n";
        return quality;
    }

} // namespace hw_sdk_mock
