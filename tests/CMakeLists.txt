project(tests_run)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# adding the Google_Tests_run target
add_executable(tests_run test.cpp)

# linking Google_Tests_run
target_link_libraries(tests_run gtest gtest_main server_lib)

include(GoogleTest)
gtest_discover_tests(tests_run)
