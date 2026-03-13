#pragma once

#include "user_profile.h"
#include "meal.h"
#include "statistics.h"
#include "data_manager.h"
#include "exceptions.h"
#include <memory>
#include <vector>
#include <optional>
#include <string>

class CalorieTracker {
private:
    std::unique_ptr<UserProfile> user;
    std::vector<std::unique_ptr<Meal>> meals;
    std::vector<std::shared_ptr<FoodItem>> foodDatabase;
    std::unique_ptr<DataManager> dataManager;
    std::string currentUsername;

    CalorieTracker();

public:
    CalorieTracker(const CalorieTracker&) = delete;
    CalorieTracker& operator=(const CalorieTracker&) = delete;

    static CalorieTracker& getInstance();

    void initialize(std::string username);
    bool loadData();
    void saveData();

    void setUserProfile(std::unique_ptr<UserProfile> newUser);
    UserProfile* getUserProfile() const noexcept;

    void addToFoodDatabase(std::shared_ptr<FoodItem> food);
    std::optional<std::shared_ptr<FoodItem>> findFoodInDatabase(std::string_view name) const;

    const std::vector<std::shared_ptr<FoodItem>>& getAllFoods() const noexcept;

    void addMeal(std::unique_ptr<Meal> meal);
    const std::vector<std::unique_ptr<Meal>>& getMeals() const noexcept;

    std::optional<Statistics::DailySummary> getTodaysSummary() const;
    double getTotalCaloriesForDay(std::chrono::system_clock::time_point day) const;

    std::string getCurrentUsername() const noexcept;

    ~CalorieTracker();
};
