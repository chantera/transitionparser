# - Try to find Eigen3 lib
#
# Once done this will define
#
#  DyNet_FOUND - system has dynet lib with correct version
#  DyNet_INCLUDE_DIR - the dynet include directory
#  DyNet_LIBRARY_DIR - the dynet library directory
#
# This module reads hints about search locations from
# the following enviroment variables:
#
# DyNet_ROOT
# DyNet_ROOT_DIR
#
if (DyNet_INCLUDE_DIR AND DyNet_LIBRARY_DIR)
  # in cache already
  set(DyNet_FOUND TRUE)

else ()

  # search first if an DyNetConfig.cmake is available in the system,
  # if successful this would set DyNet_INCLUDE_DIR and DyNet_LIBRARY_DIR
  # and the rest of the script will work as usual
  find_package(DyNet NO_MODULE QUIET)

  if(NOT DyNet_INCLUDE_DIR)
    find_path(DyNet_INCLUDE_DIR
      NAMES dynet/dynet.h
      HINTS
        ENV DyNet_ROOT
        ENV DyNet_ROOT_DIR
      PATHS
        ${CMAKE_INSTALL_PREFIX}/include
      )
  endif(NOT DyNet_INCLUDE_DIR)

  if(NOT DyNet_LIBRARY_DIR)
    find_path(DyNet_LIBRARY_DIR
      NAMES libdynet.so libdynet.dylib
      HINTS
        $ENV{DyNet_ROOT}/build/dynet
        $ENV{DyNet_ROOT_DIR}/build/dynet
      PATHS
        ${CMAKE_INSTALL_PREFIX}/lib
      )
  endif(NOT DyNet_LIBRARY_DIR)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(DyNet DEFAULT_MSG DyNet_INCLUDE_DIR DyNet_LIBRARY_DIR)

  mark_as_advanced(DyNet_INCLUDE_DIR)
  mark_as_advanced(DyNet_LIBRARY_DIR)

endif()

