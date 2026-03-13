# HW SDK Mock Library

Mock interface for GPS, Link, and Flight Controller systems with randomized responses to simulate hardware SDK behavior.

## Prerequisites

- **CMake** 3.8+
- **C++ compiler** supporting C++23

## Build

```bash
git clone https://github.com/PavelGuzenfeld/hw-sdk-mock
cd hw-sdk-mock
```

Each module (gps, link, flight-controller) is built independently:

```bash
cd <module>
cmake -B build
cmake --build build
sudo cmake --install build
```

## Run

Each module includes a demo executable:

```bash
./build/gps_demo
./build/link_demo
./build/flight_controller_demo
```

## Structure

```
hw-sdk-mock/
├── gps/
│   ├── include/gps/gps.hpp
│   ├── src/gps.cpp
│   ├── src/demo.cpp
│   └── CMakeLists.txt
├── link/
│   ├── include/link/link.hpp
│   ├── src/link.cpp
│   ├── src/demo.cpp
│   └── CMakeLists.txt
└── flight-controller/
    ├── include/flight-controller/flight_controller.hpp
    ├── src/flight_controller.cpp
    ├── src/demo.cpp
    └── CMakeLists.txt
```

## Usage

```cpp
#include "gps/gps.hpp"
#include "link/link.hpp"
#include "flight-controller/flight_controller.hpp"
```

### Linking with CMake

```cmake
find_package(gps REQUIRED)
find_package(link REQUIRED)
find_package(flight-controller REQUIRED)
target_link_libraries(your_target PRIVATE gps link flight-controller)
```
