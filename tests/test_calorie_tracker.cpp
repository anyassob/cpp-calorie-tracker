#include <gtest/gtest.h>
#include \"calorie_tracker.h\"
#include \"basic_food.h\"

class CalorieTrackerTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto& tracker = CalorieTracker::getInstance();
        tracker.initialize(\"TestUser\");
    }
};

TEST_F(CalorieTrackerTest, AddProduct) {
    auto& tracker = CalorieTracker::getInstance();
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    
    tracker.addToFoodDatabase(apple);
    
    auto found = tracker.findFoodInDatabase(\"Apple\");
    EXPECT_TRUE(found.has_value());
    EXPECT_EQ(found.value()->getName(), \"Apple\");
}

TEST_F(CalorieTrackerTest, FindNonexistentProduct) {
    auto& tracker = CalorieTracker::getInstance();
    auto found = tracker.findFoodInDatabase(\"Nonexistent\");
    EXPECT_FALSE(found.has_value());
}

TEST_F(CalorieTrackerTest, AddMeal) {
    auto& tracker = CalorieTracker::getInstance();
    auto meal = std::make_unique<Meal>(\"Lunch\");
    auto apple = std::make_unique<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    meal->addFood(std::move(apple));
    
    tracker.addMeal(std::move(meal));
    
    EXPECT_EQ(tracker.getMeals().size(), 1);
}

TEST_F(CalorieTrackerTest, UserProfile) {
    auto& tracker = CalorieTracker::getInstance();
    auto user = std::make_unique<UserProfile>(\"Test\", 70.0, 170.0, 30, 'M');
    user->setDailyCalorieGoal(2500.0);
    
    tracker.setUserProfile(std::move(user));
    
    auto profile = tracker.getUserProfile();
    ASSERT_NE(profile, nullptr);
    EXPECT_EQ(profile->getName(), \"Test\");
    EXPECT_EQ(tracker.getCurrentUsername(), \"Test\");
}

TEST_F(CalorieTrackerTest, InvalidAddNull) {
    auto& tracker = CalorieTracker::getInstance();
    EXPECT_THROW(tracker.addToFoodDatabase(nullptr), ValidationException);
    EXPECT_THROW(tracker.addMeal(nullptr), ValidationException);
}
