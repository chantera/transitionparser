cmake_minimum_required(VERSION 3.6)
project(coordparser)

# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

set(SOURCE_FILES main.cc app.cc app.h logger.cc logger.h utility.h parser.cc parser.h classifier.cc classifier.h state.cc state.h sentence.cc sentence.h greedy_parser.cc greedy_parser.h action.cc action.h token.cc token.h reader.cc reader.h arc.cc arc.h mlp_classifier.cc mlp_classifier.h feature.cc feature.h)
add_executable(coordparser ${SOURCE_FILES})

target_link_libraries(coordparser ${Boost_LIBRARIES} ${DYNET_LIBRARIES})
