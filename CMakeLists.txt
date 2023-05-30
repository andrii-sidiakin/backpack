cmake_minimum_required(VERSION 3.22)
project(BPL)

#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# setup compiler/language features
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# setup compier options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra)
    add_compile_options(-Werror)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
    add_compile_options(-Werror)
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} is not supported")
endif()

# configure global options
if(BPL_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
endif()

add_library(backpack INTERFACE)
target_include_directories(backpack INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# configure testing
option(BPL_BUILD_TESTING "Enable testing" YES)
option(BPL_INCLUDE_CTEST "Include CTest integration" NO)
if(BPL_BUILD_TESTING)
    if(BPL_INCLUDE_CTEST)
        include(CTest)
    else()
        enable_testing()
    endif()
    add_subdirectory(tests)
endif()
