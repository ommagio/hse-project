cmake_minimum_required(VERSION 3.10)
project(image_processor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    image_processor.cpp
    parser.cpp
    bmp.cpp
    filters/grayscale_filter.cpp
    filters/negative_filter.cpp
    filters/crop_filter.cpp
    filters/base_filter.cpp
    filters/sharpen_filter.cpp
    filters/edge_filter.cpp
    filters/blur_filter.cpp
    filters/sepia_filter.cpp
)

add_executable(image_processor ${SOURCE_FILES})

target_include_directories(image_processor PRIVATE .)
target_compile_options(image_processor PRIVATE
    -Wall
    -Wextra
)

enable_testing()

set(TEST_SOURCE_FILES
    tests/tests.cpp
    parser.cpp
    filters/grayscale_filter.cpp
    filters/negative_filter.cpp
    filters/crop_filter.cpp
    filters/sharpen_filter.cpp
    filters/edge_filter.cpp
    filters/blur_filter.cpp
    filters/sepia_filter.cpp
)

add_executable(image_processor_tests ${TEST_SOURCE_FILES})

target_include_directories(image_processor_tests PRIVATE .)

target_compile_options(image_processor_tests PRIVATE
    -Wall
    -Wextra
)

add_test(NAME image_processor_tests COMMAND image_processor_tests)