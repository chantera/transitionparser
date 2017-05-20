cmake_minimum_required(VERSION 2.8)
project(coordparser)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Set default build type.
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not given, defaulting to 'Dev'.")
  set(CMAKE_BUILD_TYPE "Dev" CACHE STRING "Choose the type of build." FORCE)
endif()

# Set available build types for CMake GUIs.
# A different build type can still be set by -DCMAKE_BUILD_TYPE=...
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
STRINGS "Debug" "Dev" "Release" "MinSizeRel" "RelWithDebInfo")

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(DEBUG 1)
else()
  set(DEBUG 0)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -Werror")

find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(DyNet REQUIRED)

#set(DYNET_LIBRARIES "gdynet")  # GPU
set(DYNET_LIBRARIES "dynet")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/coordparser/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/coordparser/config.h
)
add_definitions(-DHAVE_CONFIG_H)

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${Boost_INCLUDE_DIR}
  ${EIGEN3_INCLUDE_DIR}
  ${DyNet_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/spdlog/include
)

link_directories(
  ${Boost_LIBRARY_DIR}
  ${DyNet_LIBRARY_DIR}
)

add_subdirectory(coordparser)

enable_testing()
set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/external/googletest)
add_subdirectory(${GTEST_ROOT})
add_subdirectory(test)
