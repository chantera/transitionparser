cmake_minimum_required(VERSION 3.6)
project(coordparser_test)

set(CMAKE_CXX_STANDARD 14)

# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

# set(SOURCE_FILES coordparser/main.cc coordparser/app.cc coordparser/app.h coordparser/logger.cc coordparser/logger.h coordparser/utility.h coordparser/parser.cc coordparser/parser.h coordparser/classifier.cc coordparser/classifier.h coordparser/state.cc coordparser/state.h coordparser/sentence.cc coordparser/sentence.h coordparser/greedy_parser.cc coordparser/greedy_parser.h coordparser/action.cc coordparser/action.h coordparser/token.cc coordparser/token.h)
# add_executable(coordparser ${SOURCE_FILES})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(coordparser ${Boost_LIBRARIES})
