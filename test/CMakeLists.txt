include_directories(
    ${GTEST_ROOT}/googletest/include
    ${GTEST_ROOT}/googlemock/include
)

set(PROJECT_TEST_NAME "${PROJECT_NAME}_test")
add_executable(${PROJECT_TEST_NAME} main.cc transition_test.cc)
target_link_libraries(${PROJECT_TEST_NAME} transitionparser ${Boost_LIBRARIES} ${DYNET_LIBRARIES} gtest gtest_main pthread)
add_test(NAME test COMMAND ${PROJECT_TEST_NAME})
