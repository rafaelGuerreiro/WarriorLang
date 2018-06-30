cmake_minimum_required(VERSION 3.11.3)
project(warriorlang CXX)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
add_compile_options("-v")

include_directories(${LLVM_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")

add_definitions(${LLVM_DEFINITIONS})


# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

add_subdirectory(lib)
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS $ENV{CXXFLAGS}
    LINK_FLAGS $ENV{LDFLAGS}
)