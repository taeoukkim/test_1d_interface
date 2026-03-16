CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

PROJECT(Test1DInterfaceLoader)
ENABLE_LANGUAGE(C CXX)

SET(CMAKE_CXX_FLAGS "-g -m64 -O3 -std=c++20 -fPIC")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
FILE(GLOB SRCS "src/*.cpp")

# Create executable
ADD_EXECUTABLE(test_interface_loader ${SRCS})

message(STATUS "Test project configured")
message(STATUS "Output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_interface_loader")