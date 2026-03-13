#include <gtest/gtest.h>
#include "data_manager.h"
#include "user_profile.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class DataManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = "./test_data_" + std::to_string(time(nullptr));
        dm = std::make_unique<DataManager>(testDir);
    }
    
    void TearDown() override {
        if (fs::exists(testDir)) {
            fs::remove_all(testDir);
        }
    }
    
    std::unique_ptr<DataManager> dm;
    std::string testDir;
};

TEST_F(DataManagerTest, ConstructorCreatesDirectory) {
    EXPECT_TRUE(fs::exists(testDir));
}

TEST_F(DataManagerTest, SaveAndLoadUserProfile) {
    UserProfile profile("TestUser", 70.0, 170.0, 30, 'M');
    profile.setDailyCalorieGoal(2500.0);
    
    EXPECT_NO_THROW(dm->saveUserProfile(profile));
    
    auto loaded = dm->loadUserProfile("TestUser");
    EXPECT_TRUE(loaded.has_value());
    EXPECT_EQ(loaded->getName(), "TestUser");
    EXPECT_DOUBLE_EQ(loaded->getWeight(), 70.0);
    EXPECT_DOUBLE_EQ(loaded->getHeight(), 170.0);
    EXPECT_EQ(loaded->getAge(), 30);
    EXPECT_EQ(loaded->getGender(), 'M');
    
    auto goal = loaded->getDailyCalorieGoal();
    EXPECT_TRUE(goal.has_value());
    EXPECT_DOUBLE_EQ(goal.value(), 2500.0);
}

TEST_F(DataManagerTest, LoadNonExistentUserReturnsNullopt) {
    auto loaded = dm->loadUserProfile("NonExistent");
    EXPECT_FALSE(loaded.has_value());
}

TEST_F(DataManagerTest, DataExists) {
    UserProfile profile("TestUser", 70.0, 170.0, 30, 'M');
    dm->saveUserProfile(profile);
    
    EXPECT_TRUE(dm->dataExists("user_TestUser.txt"));
    EXPECT_FALSE(dm->dataExists("non_existent_file.txt"));
}
