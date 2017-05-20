# set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set (Boost_USE_MULTITHREAD ON)  # enable multithreading

set(HEADER_FILES logger.h utility.h parser.h classifier.h state.h sentence.h transition.h token.h tools.h feature.h)
set(SOURCE_FILES parser.cc classifier.cc state.cc sentence.cc transition.cc token.cc feature.cc)
add_library(coordparser ${HEADER_FILES} ${SOURCE_FILES})

add_executable(main main.cc)
target_link_libraries(main ${Boost_LIBRARIES} ${DYNET_LIBRARIES} coordparser)
