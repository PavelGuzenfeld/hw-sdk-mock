cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Built as subdirectory — inherit settings from parent
else()
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../VERSION" PROJECT_VERSION)
    string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
    project(gps VERSION ${PROJECT_VERSION})
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(FetchContent)
    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.1.4 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(fmt)
endif()

add_compile_options(-Wformat -Wall -Wextra -pedantic -Werror -Wconversion -Wshadow -Wunreachable-code -Wunused -Wunused-function)

set(SOURCE_FILES src/gps.cpp)

add_library(gps SHARED ${SOURCE_FILES})
target_include_directories(gps
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(gps PUBLIC fmt::fmt)

add_executable(gps_demo src/demo.cpp)
target_link_libraries(gps_demo gps)

install(TARGETS gps EXPORT gps LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin INCLUDES DESTINATION include)
install(EXPORT gps FILE gpsConfig.cmake DESTINATION lib/cmake/gps)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS gps_demo RUNTIME DESTINATION bin)
