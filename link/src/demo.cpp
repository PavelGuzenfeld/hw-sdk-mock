#include "link/link.hpp"

#include <fmt/format.h>

int main()
{
    hw_sdk_mock::Link link;

    for (int i = 0; i < 5; ++i)
    {
        auto quality = link.getSignalQuality();
        fmt::print("Signal Quality: {}\n", static_cast<int>(quality));
    }

    return 0;
}
