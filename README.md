# HW SDK Mock Library

Mock interface for GPS, Link, and Flight Controller systems with realistic randomized behavior to simulate hardware SDK responses.

## Prerequisites

- **CMake** 3.14+
- **C++ compiler** supporting C++23
- **fmt** (fetched automatically via CMake FetchContent)

## Build

```bash
git clone https://github.com/PavelGuzenfeld/hw-sdk-mock
cd hw-sdk-mock
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Run

```bash
./build/gps/gps_demo
./build/link/link_demo
./build/flight-controller/flight-controller_demo
```

## Structure

```
hw-sdk-mock/
├── CMakeLists.txt              # Top-level build (fetches fmt)
├── VERSION                     # Single source of truth for versioning
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
