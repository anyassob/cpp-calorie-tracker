#include <gtest/gtest.h>
#include "../include/basic_food.h"
#include "../include/exceptions.h"

TEST(BasicFoodTest, ConstructorAndGetters) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);
    
    EXPECT_EQ(apple.getName(), "Apple");
    EXPECT_DOUBLE_EQ(apple.getProtein(), 0.3);
    EXPECT_DOUBLE_EQ(apple.getFat(), 0.2);
    EXPECT_DOUBLE_EQ(apple.getCarbs(), 14.0);
    EXPECT_DOUBLE_EQ(apple.getPortionMultiplier(), 1.0);
}


TEST(BasicFoodTest, CaloriesCalculation) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);

    EXPECT_NEAR(apple.getCalories(), 59.0, 0.1);
}


TEST(BasicFoodTest, PortionMultiplier) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 2.0);
    
    EXPECT_DOUBLE_EQ(apple.getProtein(), 0.6);
    EXPECT_DOUBLE_EQ(apple.getFat(), 0.4);
    EXPECT_DOUBLE_EQ(apple.getCarbs(), 28.0);
    EXPECT_NEAR(apple.getCalories(), 118.0, 0.1);
}


TEST(BasicFoodTest, UpdatePortion) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);
    apple.updatePortion(2.0);
    
    EXPECT_DOUBLE_EQ(apple.getPortionMultiplier(), 2.0);
    EXPECT_DOUBLE_EQ(apple.getProtein(), 0.6);
}


TEST(BasicFoodTest, SetPortionMultiplier) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);
    apple.setPortionMultiplier(3.0);
    
    EXPECT_DOUBLE_EQ(apple.getPortionMultiplier(), 3.0);
    EXPECT_DOUBLE_EQ(apple.getProtein(), 0.9);
}

TEST(BasicFoodTest, NegativeValuesThrow) {
    EXPECT_THROW(BasicFood("Bad", -1.0, 0.2, 14.0, 1.0), ValidationException);
    EXPECT_THROW(BasicFood("Bad", 0.3, -0.2, 14.0, 1.0), ValidationException);
    EXPECT_THROW(BasicFood("Bad", 0.3, 0.2, -14.0, 1.0), ValidationException);
}

TEST(BasicFoodTest, InvalidMultiplierThrows) {
    EXPECT_THROW(BasicFood("Bad", 0.3, 0.2, 14.0, 0.0), ValidationException);
    EXPECT_THROW(BasicFood("Bad", 0.3, 0.2, 14.0, -1.0), ValidationException);
}

TEST(BasicFoodTest, Description) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);
    std::string desc = apple.getDescription();
    
    EXPECT_TRUE(desc.find("Apple") != std::string::npos);
    EXPECT_TRUE(desc.find("59.0 kcal") != std::string::npos);
}


TEST(BasicFoodTest, Clone) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 2.0);
    auto cloned = apple.clone();
    
    EXPECT_EQ(cloned->getName(), apple.getName());
    EXPECT_DOUBLE_EQ(cloned->getCalories(), apple.getCalories());
    EXPECT_DOUBLE_EQ(dynamic_cast<BasicFood*>(cloned.get())->getPortionMultiplier(), 2.0);
}


TEST(BasicFoodTest, GetNutrient) {
    BasicFood apple("Apple", 0.3, 0.2, 14.0, 1.0);
    
    auto calories = apple.getNutrient("calories");
    auto protein = apple.getNutrient("protein");
    auto fat = apple.getNutrient("fat");
    auto carbs = apple.getNutrient("carbs");
    auto unknown = apple.getNutrient("vitamins");
    
    EXPECT_TRUE(calories.has_value());
    EXPECT_TRUE(protein.has_value());
    EXPECT_TRUE(fat.has_value());
    EXPECT_TRUE(carbs.has_value());
    EXPECT_FALSE(unknown.has_value());
    
    EXPECT_NEAR(calories.value(), 59.0, 0.1);
}
