#include "gps/gps.hpp"

#include <fmt/format.h>

int main()
{
    hw_sdk_mock::Gps gps;

    for (int i = 0; i < 5; ++i)
    {
        auto location = gps.getLocation();
        auto quality = gps.getSignalQuality();

        if (location)
        {
            fmt::print("Location: {:.6f}, {:.6f}, {:.6f} meters\n",
                       location->latitude, location->longitude, location->altitude);
        }
        else
        {
            fmt::print("Location: unavailable\n");
        }
        fmt::print("Signal Quality: {}\n", static_cast<int>(quality));
    }

    return 0;
}
