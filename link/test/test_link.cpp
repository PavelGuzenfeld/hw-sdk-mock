#include <doctest/doctest.h>
#include "link/link.hpp"

#include <utility>

using hw_sdk_mock::Link;

TEST_CASE("signal quality is always a valid enum value")
{
    Link link;

    for (int i = 0; i < 100; ++i)
    {
        auto q = link.getSignalQuality();
        int val = static_cast<int>(q);
        CHECK(val >= 0);
        CHECK(val <= 4);
    }
}

TEST_CASE("all signal quality values are reachable")
{
    bool seen[5] = {};

    // Run enough iterations to see all values (uniform distribution over 5 values)
    Link link;
    for (int i = 0; i < 500; ++i)
    {
        auto q = link.getSignalQuality();
        seen[static_cast<int>(q)] = true;
    }

    CHECK(seen[0]); // NO_SIGNAL
    CHECK(seen[1]); // POOR
    CHECK(seen[2]); // FAIR
    CHECK(seen[3]); // GOOD
    CHECK(seen[4]); // EXCELLENT
}

TEST_CASE("link can be move-constructed")
{
    Link link1;
    Link link2 = std::move(link1);

    auto q = link2.getSignalQuality();
    int val = static_cast<int>(q);
    CHECK(val >= 0);
    CHECK(val <= 4);
}
