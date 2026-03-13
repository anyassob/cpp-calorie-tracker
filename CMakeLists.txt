cmake_minimum_required(VERSION 3.20)
project(caloristrecker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

set(FETCHCONTENT_QUIET OFF)

FetchContent_MakeAvailable(googletest)

add_executable(caloristrecker
        src/main.cpp
        src/food_item.cpp
        src/basic_food.cpp
        src/user_profile.cpp
        src/statistics.cpp
        src/data_manager.cpp
)

enable_testing()

add_executable(tests_user_profile
        tests/test_user_profile.cpp
        src/user_profile.cpp
)

target_link_libraries(tests_user_profile
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_user_profile)