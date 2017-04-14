cmake_minimum_required(VERSION 3.6)
project(coordparser_test)

set(CMAKE_CXX_STANDARD 14)

# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

# set(SOURCE_FILES src/main.cc src/app.cc src/app.h src/logger.cc src/logger.h src/utility.h src/parser.cc src/parser.h src/classifier.cc src/classifier.h src/state.cc src/state.h src/sentence.cc src/sentence.h src/greedy_parser.cc src/greedy_parser.h src/action.cc src/action.h src/token.cc src/token.h)
# add_executable(coordparser ${SOURCE_FILES})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(coordparser ${Boost_LIBRARIES})
