cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Built as subdirectory — inherit settings from parent
else()
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../VERSION" PROJECT_VERSION)
    string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
    project(link VERSION ${PROJECT_VERSION})
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(FetchContent)
    FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.1.4 GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(fmt)
endif()

add_compile_options(-Wformat -Wall -Wextra -pedantic -Werror -Wconversion -Wshadow -Wunreachable-code -Wunused -Wunused-function)

set(SOURCE_FILES src/link.cpp)

add_library(link SHARED ${SOURCE_FILES})
target_include_directories(link
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(link PUBLIC fmt::fmt)

add_executable(link_demo src/demo.cpp)
target_link_libraries(link_demo link)

install(TARGETS link EXPORT link LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin INCLUDES DESTINATION include)
install(EXPORT link FILE linkConfig.cmake DESTINATION lib/cmake/link)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS link_demo RUNTIME DESTINATION bin)

if(TARGET doctest_with_main)
    add_executable(link_test test/test_link.cpp)
    target_link_libraries(link_test link doctest_with_main)
    add_test(NAME link_test COMMAND link_test)
endif()
