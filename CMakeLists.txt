cmake_minimum_required(VERSION 3.6)
project(coordparser)


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

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${EIGEN_INCLUDE_DIR}
    ${DYNET_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${DYNET_LIBRARY_DIR}
)


add_subdirectory(src)
# add_subdirectory(test)
