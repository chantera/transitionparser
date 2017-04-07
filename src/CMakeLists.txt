cmake_minimum_required(VERSION 3.6)
project(coordparser)

set(CMAKE_CXX_STANDARD 14)

# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

set(SOURCE_FILES main.cc app.cc app.h logger.cc logger.h utility.h parser.cc parser.h classifier.cc classifier.h state.cc state.h sentence.cc sentence.h greedy_parser.cc greedy_parser.h action.cc action.h token.cc token.h conll_reader.cc conll_reader.h reader.cc reader.h perceptron.cc perceptron.h)
add_executable(coordparser ${SOURCE_FILES})

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    target_link_libraries(coordparser ${Boost_LIBRARIES})
endif()