# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

set(HEADER_FILES app.h logger.h utility.h parser.h classifier.h state.h sentence.h action.h token.h reader.h arc.h feature.h)
set(SOURCE_FILES app.cc logger.cc parser.cc classifier.cc state.cc sentence.cc action.cc token.cc reader.cc arc.cc feature.cc)
add_library(coordparser ${HEADER_FILES} ${SOURCE_FILES})

add_executable(main main.cc)
target_link_libraries(main ${Boost_LIBRARIES} ${DYNET_LIBRARIES} coordparser)
