cmake_minimum_required(VERSION 3.6)
project(coordparser)

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

function(check_option name)
  if(DEFINED ENV{${name}} AND NOT DEFINED ${name})
    set(${name} "$ENV{${name}}")
  endif()
  if(${name})
    MESSAGE("-- ${name}: ${${name}}")
  else()
    MESSAGE(FATAL_ERROR "Please give the variable `${name}`.")
  endif()
endfunction()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -Werror")
endif()

find_package(Boost REQUIRED)
set(EIGEN_INCLUDE_DIR /usr/local/Cellar/eigen/HEAD-a038e46f2d3e/include/eigen3)
set(DYNET_INCLUDE_DIR /Users/hiroki/work/dynet)
set(DYNET_LIBRARY_DIR /Users/hiroki/work/dynet/build/dynet)
MESSAGE("EIGEN_INCLUDE_DIR=${EIGEN_INCLUDE_DIR}")
MESSAGE("DYNET_INCLUDE_DIR=${DYNET_INCLUDE_DIR}")
MESSAGE("DYNET_LIBRARY_DIR=${DYNET_LIBRARY_DIR}")
#check_option(DYNET_INCLUDE_DIR)
#check_option(DYNET_LIBRARY_DIR)

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
    ${Boost_INCLUDE_DIRS}
    ${EIGEN_INCLUDE_DIR}
    ${DYNET_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/spdlog/include
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${DYNET_LIBRARY_DIR}
)

add_subdirectory(coordparser)

enable_testing()
set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/external/googletest)
add_subdirectory(${GTEST_ROOT})
add_subdirectory(test)
