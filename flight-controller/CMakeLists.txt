cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Built as subdirectory — inherit settings from parent
else()
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../VERSION" PROJECT_VERSION)
    string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
    project(flight-controller VERSION ${PROJECT_VERSION})
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(FetchContent)
    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.1.4 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(fmt)
endif()

add_compile_options(-Wformat -Wall -Wextra -pedantic -Werror -Wconversion -Wshadow -Wunreachable-code -Wunused -Wunused-function)

set(SOURCE_FILES src/flight_controller.cpp)

add_library(flight-controller SHARED ${SOURCE_FILES})
target_include_directories(flight-controller
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(flight-controller PUBLIC fmt::fmt)

add_executable(flight-controller_demo src/demo.cpp)
target_link_libraries(flight-controller_demo flight-controller)

install(TARGETS flight-controller EXPORT flight-controller LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin INCLUDES DESTINATION include)
install(EXPORT flight-controller FILE flight-controllerConfig.cmake DESTINATION lib/cmake/flight-controller)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS flight-controller_demo RUNTIME DESTINATION bin)

if(TARGET doctest_with_main)
    add_executable(flight-controller_test test/test_flight_controller.cpp)
    target_link_libraries(flight-controller_test flight-controller doctest_with_main)
    add_test(NAME flight-controller_test COMMAND flight-controller_test)
endif()
