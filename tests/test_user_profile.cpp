#include <gtest/gtest.h>
#include "user_profile.h"
#include "exceptions.h"

TEST(UserProfileTest, BMRCalculation) {
UserProfile user("Test", 70, 170, 30, 'M');
double bmr = user.calculateBMR();
EXPECT_NEAR(bmr, 1617.5, 0.1);
}

TEST(UserProfileTest, BMRCalculationFemale) {
UserProfile user("Test", 60, 165, 25, 'F');
double bmr = user.calculateBMR();
EXPECT_NEAR(bmr, (10*60) + (6.25*165) - (5*25) - 161, 0.1);
}

TEST(UserProfileTest, DailyGoalOptional) {
UserProfile user("Test", 70, 170, 30, 'M');
EXPECT_FALSE(user.getDailyCalorieGoal().has_value());

user.setDailyCalorieGoal(2500);
EXPECT_TRUE(user.getDailyCalorieGoal().has_value());
EXPECT_EQ(user.getDailyCalorieGoal().value(), 2500);
}

TEST(UserProfileTest, InvalidWeightThrows) {
EXPECT_THROW(UserProfile("Test", -70, 170, 30, 'M'), ValidationException);
EXPECT_THROW(UserProfile("Test", 600, 170, 30, 'M'), ValidationException);
}

TEST(UserProfileTest, InvalidHeightThrows) {
EXPECT_THROW(UserProfile("Test", 70, -10, 30, 'M'), ValidationException);
EXPECT_THROW(UserProfile("Test", 70, 400, 30, 'M'), ValidationException);
}

TEST(UserProfileTest, InvalidAgeThrows) {
EXPECT_THROW(UserProfile("Test", 70, 170, -5, 'M'), ValidationException);
EXPECT_THROW(UserProfile("Test", 70, 170, 200, 'M'), ValidationException);
}

TEST(UserProfileTest, InvalidGenderThrows) {
EXPECT_THROW(UserProfile("Test", 70, 170, 30, 'X'), ValidationException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}