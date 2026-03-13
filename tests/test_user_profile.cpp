#include <gtest/gtest.h>
#include "user_profile.h"
#include "exceptions.h"

TEST(UserProfileTest, Constructor) {
    UserProfile user("TestUser", 70.0, 170.0, 30, 'M');
    
    EXPECT_EQ(user.getName(), "TestUser");
    EXPECT_DOUBLE_EQ(user.getWeight(), 70.0);
    EXPECT_DOUBLE_EQ(user.getHeight(), 170.0);
    EXPECT_EQ(user.getAge(), 30);
    EXPECT_EQ(user.getGender(), 'M');
}

TEST(UserProfileTest, BMRMale) {
    UserProfile user("Male", 70.0, 170.0, 30, 'M');
    EXPECT_NEAR(user.calculateBMR(), 1617.5, 0.1);
}

TEST(UserProfileTest, BMRFemale) {
    UserProfile user("Female", 60.0, 165.0, 28, 'F');
    EXPECT_NEAR(user.calculateBMR(), 1330.25, 0.1);
}

TEST(UserProfileTest, DailyGoal) {
    UserProfile user("Test", 70.0, 170.0, 30, 'M');
    
    auto goal = user.getDailyCalorieGoal();
    EXPECT_FALSE(goal.has_value());
    
    user.setDailyCalorieGoal(2500.0);
    goal = user.getDailyCalorieGoal();
    
    EXPECT_TRUE(goal.has_value());
    EXPECT_DOUBLE_EQ(goal.value(), 2500.0);
}

TEST(UserProfileTest, UpdateWeight) {
    UserProfile user("Test", 70.0, 170.0, 30, 'M');
    
    user.updateWeight(75.0);
    EXPECT_DOUBLE_EQ(user.getWeight(), 75.0);
}

TEST(UserProfileTest, UpdateHeight) {
    UserProfile user("Test", 70.0, 170.0, 30, 'M');
    
    user.updateHeight(175.0);
    EXPECT_DOUBLE_EQ(user.getHeight(), 175.0);
}

TEST(UserProfileTest, InvalidWeightThrows) {
    EXPECT_THROW(UserProfile("Bad", -70.0, 170.0, 30, 'M'), ValidationException);
    EXPECT_THROW(UserProfile("Bad", 0.0, 170.0, 30, 'M'), ValidationException);
    EXPECT_THROW(UserProfile("Bad", 600.0, 170.0, 30, 'M'), ValidationException);
}

TEST(UserProfileTest, InvalidHeightThrows) {
    EXPECT_THROW(UserProfile("Bad", 70.0, -170.0, 30, 'M'), ValidationException);
    EXPECT_THROW(UserProfile("Bad", 70.0, 0.0, 30, 'M'), ValidationException);
    // 300 см — допустимо, поэтому не должно быть исключения
    EXPECT_NO_THROW(UserProfile("Good", 70.0, 300.0, 30, 'M'));
}

TEST(UserProfileTest, InvalidAgeThrows) {
    EXPECT_THROW(UserProfile("Bad", 70.0, 170.0, -30, 'M'), ValidationException);
    EXPECT_THROW(UserProfile("Bad", 70.0, 170.0, 0, 'M'), ValidationException);
    EXPECT_THROW(UserProfile("Bad", 70.0, 170.0, 200, 'M'), ValidationException);
}

TEST(UserProfileTest, InvalidGenderThrows) {
    EXPECT_THROW(UserProfile("Bad", 70.0, 170.0, 30, 'X'), ValidationException);
}

TEST(UserProfileTest, UpdateWeightWithInvalidValue) {
    UserProfile user("Test", 70.0, 170.0, 30, 'M');
    
    EXPECT_THROW(user.updateWeight(-10.0), ValidationException);
    EXPECT_THROW(user.updateWeight(0.0), ValidationException);
    EXPECT_THROW(user.updateWeight(600.0), ValidationException);
}

TEST(UserProfileTest, UpdateHeightWithInvalidValue) {
    UserProfile user("Test", 70.0, 170.0, 30, 'M');
    
    EXPECT_THROW(user.updateHeight(-10.0), ValidationException);
    EXPECT_THROW(user.updateHeight(0.0), ValidationException);

    EXPECT_NO_THROW(user.updateHeight(300.0));
}
