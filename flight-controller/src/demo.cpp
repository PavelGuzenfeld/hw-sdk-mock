#include "flight-controller/flight_controller.hpp"


int main()
{
    hw_sdk_mock::FlightController fc;
    fc.arm();
    auto const hight = 10.0;
    fc.takeOff(hight);
    fc.goTo(10.0, 10.0, hight);
    fc.land();
    fc.disarm();

    return 0;
}
