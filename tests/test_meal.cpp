#include <gtest/gtest.h>
#include "meal.h"
#include "basic_food.h"
#include "exceptions.h"

TEST(MealTest, Constructor) {
    Meal meal("Breakfast");
    EXPECT_EQ(meal.getName(), "Breakfast");
}

TEST(MealTest, AddFood) {
    Meal meal("Breakfast");
    auto apple = std::make_unique<BasicFood>("Apple", 0.3, 0.2, 14.0, 1.0);
    
    meal.addFood(std::move(apple));
    
    EXPECT_NEAR(meal.getTotalCalories(), 59.0, 0.1);
}

TEST(MealTest, MultipleFoods) {
    Meal meal("Breakfast");
    auto apple = std::make_unique<BasicFood>("Apple", 0.3, 0.2, 14.0, 1.0);
    auto bread = std::make_unique<BasicFood>("Bread", 9.0, 3.0, 49.0, 1.0);
    
    meal.addFood(std::move(apple));
    meal.addFood(std::move(bread));
    
    EXPECT_NEAR(meal.getTotalCalories(), 59.0 + 259.0, 1.0);
    EXPECT_NEAR(meal.getTotalProtein(), 0.3 + 9.0, 0.1);
    EXPECT_NEAR(meal.getTotalFat(), 0.2 + 3.0, 0.1);
    EXPECT_NEAR(meal.getTotalCarbs(), 14.0 + 49.0, 0.1);
}

TEST(MealTest, EmptyMeal) {
    Meal meal("Empty");
    EXPECT_DOUBLE_EQ(meal.getTotalCalories(), 0.0);
    EXPECT_DOUBLE_EQ(meal.getTotalProtein(), 0.0);
    EXPECT_DOUBLE_EQ(meal.getTotalFat(), 0.0);
    EXPECT_DOUBLE_EQ(meal.getTotalCarbs(), 0.0);
}

TEST(MealTest, InvalidFoodThrows) {
    Meal meal("Breakfast");
    EXPECT_THROW(meal.addFood(nullptr), ValidationException);
}
