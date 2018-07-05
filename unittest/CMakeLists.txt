# Download and unpack googletest at configure time
configure_file(${CMAKE_SOURCE_DIR}/cmake/GoogleTest-CMakeLists.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
    ${CMAKE_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(${WARRIORLANG_UNITTEST_TARGET}
    "Parser/LexerTest.cpp")

# target_compile_features(${WARRIORLANG_UNITTEST_TARGET} PRIVATE cxx_auto_type)
set_target_properties(${WARRIORLANG_UNITTEST_TARGET} PROPERTIES
    COMPILE_FLAGS ${WARRIORLANG_DEFAULT_COMPILE_FLAGS}
    LINK_FLAGS ${WARRIORLANG_DEFAULT_LINK_FLAGS})

target_link_libraries(${WARRIORLANG_UNITTEST_TARGET} gtest_main warriorlang::${WARRIORLANG_LIBRARIES_TARGET})