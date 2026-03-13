#include "flight-controller/flight_controller.hpp"


int main()
{
    hw_sdk_mock::FlightController fc;
    fc.arm();
    auto const height = 10.0;
    fc.takeOff(height);
    fc.goTo(10.0, 10.0, height);
    fc.land();
    fc.disarm();

    return 0;
}
