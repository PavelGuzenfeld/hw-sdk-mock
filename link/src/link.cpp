#include "link/link.hpp"

#include <random>
#include <string_view>

#include <fmt/format.h>

namespace hw_sdk_mock
{

    namespace
    {
        Link::SignalQuality getRandomSignalQuality()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 4);
            return static_cast<Link::SignalQuality>(dis(gen));
        }

        std::string_view signalQualityToString(Link::SignalQuality quality)
        {
            switch (quality)
            {
            case Link::SignalQuality::NO_SIGNAL:
                return "NO_SIGNAL";
            case Link::SignalQuality::POOR:
                return "POOR";
            case Link::SignalQuality::FAIR:
                return "FAIR";
            case Link::SignalQuality::GOOD:
                return "GOOD";
            case Link::SignalQuality::EXCELLENT:
                return "EXCELLENT";
            default:
                return "UNKNOWN_SIGNAL_QUALITY";
            }
        }
    }

    Link::SignalQuality Link::getSignalQuality()
    {
        SignalQuality quality = getRandomSignalQuality();
        fmt::print("Link Signal Quality: {}\n", signalQualityToString(quality));
        return quality;
    }

} // namespace hw_sdk_mock
