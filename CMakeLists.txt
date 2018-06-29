cmake_minimum_required(VERSION 3.11.3)
project(warriorlang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wno-error=format= -Wno-error=format -Wno-error=format-extra-args")
# set(EXE_CFLAGS "-std=c++11 -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D_GNU_SOURCE -fno-exceptions -fno-rtti  -Werror=strict-prototypes -Werror=old-style-definition -Werror=type-limits -Wno-missing-braces")
# set(EXE_LDFLAGS "-static -static-libgcc -static-libstdc++")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(SOURCE "src/main.cpp")

# Now build our tools
add_executable(${PROJECT_NAME} ${SOURCE})
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS $ENV{CXXFLAGS}
    LINK_FLAGS $ENV{LDFLAGS}
)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} ${llvm_libs})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)