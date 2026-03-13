cmake_minimum_required(VERSION 3.20)
project(calorie_tracker)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(calorie_library STATIC
        src/food_item.cpp
        src/basic_food.cpp
        src/recipe.cpp
        src/meal.cpp
        src/user_profile.cpp
        src/statistics.cpp
        src/calorie_tracker.cpp
        src/data_manager.cpp
)

target_include_directories(calorie_library PRIVATE include)

add_executable(calorie_tracker
        src/main.cpp
)

target_include_directories(calorie_tracker PRIVATE include)

target_link_libraries(calorie_tracker PRIVATE calorie_library)

enable_testing()

add_executable(tests_calorie_tracker
        tests/test_basic_food.cpp
        tests/test_recipe.cpp
        tests/test_meal.cpp
        tests/test_calorie_tracker.cpp
        tests/test_statistics.cpp
        tests/test_user_profile.cpp
        tests/test_data_manager.cpp
)

target_include_directories(tests_calorie_tracker PRIVATE include)

target_link_libraries(tests_calorie_tracker PRIVATE
        calorie_library
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_calorie_tracker)
