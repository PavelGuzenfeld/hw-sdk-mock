#include "flight-controller/flight_controller.hpp"


int main()
{
    hw_sdk_mock::FlightController fc;
    auto const hight = 10.0;
    fc.arm(hight);
    fc.disarm();
    fc.takeOff();
    return 0;
}
