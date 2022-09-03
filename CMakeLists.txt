cmake_minimum_required(VERSION 3.16)
project(revealer)

set(CMAKE_CXX_STANDARD 17)

###### Dependencies #####
include(${CMAKE_SOURCE_DIR}/cmake/fetch_microtar.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/fetch_gtest.cmake)
find_package(Boost 1.75.0 REQUIRED COMPONENTS filesystem program_options)
include(${CMAKE_SOURCE_DIR}/cmake/fetch_fmt.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/fetch_rangev3.cmake)

set(SRC
    src/argument_parsing/parse_arguments.cpp
    src/collectors/disk_usage_collector.cpp
    src/collectors/path_collector.cpp
    src/collect_tarball_items.cpp
    src/create_tarball.cpp
)

set(LIB_NAME ${PROJECT_NAME}_lib)
set(EXEC_NAME ${PROJECT_NAME})

# enable all warnings and treat them as errors
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_library(${LIB_NAME} STATIC ${SRC})
target_link_libraries(${LIB_NAME} PUBLIC microtar fmt::fmt Boost::program_options Boost::filesystem range-v3)
target_include_directories(${LIB_NAME} PUBLIC src)

add_executable(${EXEC_NAME} src/main.cpp)
target_link_libraries(${EXEC_NAME} ${LIB_NAME})

enable_testing()
add_subdirectory(test)
