cmake_minimum_required(VERSION 3.11.3)
project(warriorlang VERSION 0.0.1 LANGUAGES CXX)

##############################
# CMake default configurations
##############################

# This enables assertions for Release builds.
# https://stackoverflow.com/questions/22140520/how-to-enable-assert-in-cmake-release-mode
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

# Setting THREADS_PREFER_PTHREAD_FLAG and finding package Threads resolves:
# /usr/bin/ld: ../../gtest-1.7.0/libgtest.a(gtest-all.cc.o): undefined reference to symbol     'pthread_key_delete@@GLIBC_2.2.5'
# /lib/x86_64-linux-gnu/libpthread.so.0: error adding symbols: DSO missing from command line
# https://stackoverflow.com/questions/25617839/undefined-reference-to-symbol-pthread-key-deleteglibc-2-2-5
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(LLVM REQUIRED CONFIG)

if (CMAKE_VERBOSE_MAKEFILE)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")

# Use -DCMAKE_VERBOSE_MAKEFILE=ON to make compiler verbose as well.
if (CMAKE_VERBOSE_MAKEFILE)
    add_compile_options("-v")
endif()

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/RPATH-handling#always-full-rpath
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" "${LLVM_LIB_PATH}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


##################
# Custom variables
##################
SET(WARRIORLANG_DEFAULT_COMPILE_FLAGS $ENV{CXXFLAGS})
SET(WARRIORLANG_DEFAULT_LINK_FLAGS $ENV{LDFLAGS})

set(WARRIORLANG_DEFAULT_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")

SET(WARRIORLANG_LIBRARIES_TARGET warriorlang_lib)
SET(WARRIORLANG_EXECUTABLE_TARGET warriorlang)
SET(WARRIORLANG_UNITTEST_TARGET warriorlang_unittest)

if (CMAKE_VERBOSE_MAKEFILE)
    message(STATUS "WARRIORLANG_DEFAULT_COMPILE_FLAGS: ${WARRIORLANG_DEFAULT_COMPILE_FLAGS}")
    message(STATUS "WARRIORLANG_DEFAULT_LINK_FLAGS: ${WARRIORLANG_DEFAULT_LINK_FLAGS}")
    message(STATUS "WARRIORLANG_DEFAULT_INCLUDE_DIRS: ${WARRIORLANG_DEFAULT_INCLUDE_DIRS}")
    message(STATUS "WARRIORLANG_LIBRARIES_TARGET: ${WARRIORLANG_LIBRARIES_TARGET}")
    message(STATUS "WARRIORLANG_EXECUTABLE_TARGET: ${WARRIORLANG_EXECUTABLE_TARGET}")
    message(STATUS "WARRIORLANG_UNITTEST_TARGET: ${WARRIORLANG_UNITTEST_TARGET}")
endif()

####################################
# Build WARRIORLANG_LIBRARIES_TARGET
####################################
add_library(${WARRIORLANG_LIBRARIES_TARGET}
    "lib/Frontend/Tokenizer.cpp"
    "lib/AST/StringLiteralAST.cpp"
    "lib/AST/FunctionMetadataAST.cpp"
    "lib/AST/UInt16LiteralAST.cpp"
    "lib/AST/LocalVariableExpressionAST.cpp"
    "lib/AST/BooleanLiteralAST.cpp"
    "lib/AST/Int32LiteralAST.cpp"
    "lib/AST/FunctionAST.cpp"
    "lib/AST/BinaryExpressionAST.cpp"
    "lib/AST/Int16LiteralAST.cpp"
    "lib/AST/UInt32LiteralAST.cpp"
    "lib/AST/Float64LiteralAST.cpp"
    "lib/AST/Int64LiteralAST.cpp"
    "lib/AST/CharacterLiteralAST.cpp"
    "lib/AST/TypeMetadataAST.cpp"
    "lib/AST/Int8LiteralAST.cpp"
    "lib/AST/ClosureLiteralAST.cpp"
    "lib/AST/UnaryExpressionAST.cpp"
    "lib/AST/ClosureMetadataAST.cpp"
    "lib/AST/FieldMetadataAST.cpp"
    "lib/AST/Float32LiteralAST.cpp"
    "lib/AST/FunctionCallExpressionAST.cpp"
    "lib/AST/UInt64LiteralAST.cpp"
    "lib/AST/UInt8LiteralAST.cpp"
    "lib/AST/ArgumentMetadataAST.cpp")

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(warriorlang::${WARRIORLANG_LIBRARIES_TARGET} ALIAS ${WARRIORLANG_LIBRARIES_TARGET})
target_include_directories(${WARRIORLANG_LIBRARIES_TARGET}
    PUBLIC
        $<INSTALL_INTERFACE:include> 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set_target_properties(${WARRIORLANG_LIBRARIES_TARGET} PROPERTIES
    COMPILE_FLAGS ${WARRIORLANG_DEFAULT_COMPILE_FLAGS}
    LINK_FLAGS ${WARRIORLANG_DEFAULT_LINK_FLAGS})

#####################################
# Build WARRIORLANG_EXECUTABLE_TARGET
#####################################

add_executable(${WARRIORLANG_EXECUTABLE_TARGET} "main.cpp")
target_link_libraries(${WARRIORLANG_EXECUTABLE_TARGET} warriorlang::${WARRIORLANG_LIBRARIES_TARGET} ${llvm_libs})
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS ${WARRIORLANG_DEFAULT_COMPILE_FLAGS}
    LINK_FLAGS ${WARRIORLANG_DEFAULT_LINK_FLAGS})

install(TARGETS ${WARRIORLANG_EXECUTABLE_TARGET} DESTINATION "bin")

###################################
# Build WARRIORLANG_UNITTEST_TARGET
###################################

add_subdirectory(unittest)