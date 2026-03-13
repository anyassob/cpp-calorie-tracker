#include <gtest/gtest.h>
#include "statistics.h"
#include "meal.h"
#include "basic_food.h"
#include <chrono>

class StatisticsTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto meal1 = std::make_unique<Meal>("Breakfast");
        auto apple = std::make_unique<BasicFood>("Apple", 0.3, 0.2, 14.0, 1.0);
        meal1->addFood(std::move(apple));
        
        auto meal2 = std::make_unique<Meal>("Lunch");
        auto chicken = std::make_unique<BasicFood>("Chicken", 31.0, 3.6, 0.0, 1.0);
        meal2->addFood(std::move(chicken));
        
        meals.push_back(std::move(meal1));
        meals.push_back(std::move(meal2));
    }
    
    std::vector<std::unique_ptr<Meal>> meals;
};

TEST_F(StatisticsTest, DailySummary) {
    Statistics stats(meals);
    auto summary = stats.getDailySummary(std::chrono::system_clock::now());
    
    EXPECT_NEAR(summary.totalCalories, 215.4, 0.1);
    EXPECT_EQ(summary.mealCount, 2);
    EXPECT_NEAR(summary.totalProtein, 0.3 + 31.0, 0.1);
    EXPECT_NEAR(summary.totalFat, 0.2 + 3.6, 0.1);
    EXPECT_NEAR(summary.totalCarbs, 14.0 + 0.0, 0.1);
}

TEST_F(StatisticsTest, GetLargestMeal) {
    Statistics stats(meals);
    auto largest = stats.getLargestMeal();
    
    EXPECT_TRUE(largest.has_value());
    EXPECT_EQ(largest->get().getName(), "Lunch");
}

TEST_F(StatisticsTest, EmptyStatistics) {
    std::vector<std::unique_ptr<Meal>> emptyMeals;
    Statistics stats(emptyMeals);
    
    auto summary = stats.getDailySummary(std::chrono::system_clock::now());
    EXPECT_DOUBLE_EQ(summary.totalCalories, 0.0);
    EXPECT_EQ(summary.mealCount, 0);
    EXPECT_DOUBLE_EQ(summary.totalProtein, 0.0);
    EXPECT_DOUBLE_EQ(summary.totalFat, 0.0);
    EXPECT_DOUBLE_EQ(summary.totalCarbs, 0.0);
    
    auto largest = stats.getLargestMeal();
    EXPECT_FALSE(largest.has_value());
}

TEST_F(StatisticsTest, ProgressToGoal) {
    Statistics stats(meals);
    auto progress = stats.getProgressToGoal(2000.0, std::chrono::system_clock::now());
    
    EXPECT_TRUE(progress.has_value());
    EXPECT_NEAR(progress.value(), 215.4 / 2000.0 * 100.0, 0.1);
}

TEST_F(StatisticsTest, ZeroGoal) {
    Statistics stats(meals);
    auto progress = stats.getProgressToGoal(0.0, std::chrono::system_clock::now());
    
    EXPECT_FALSE(progress.has_value());
}
