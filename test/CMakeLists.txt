set(LINK_LIBARIES ${LIB_NAME} gmock GTest::gtest_main Boost::filesystem)

add_executable(test_file_creation_listener file_creation_listener_test.cpp)
target_link_libraries(test_file_creation_listener ${LINK_LIBARIES})
add_test(NAME test_file_creation_listener COMMAND test_file_creation_listener)
