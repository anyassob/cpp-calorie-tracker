#include "calorie_tracker.h"
#include <iostream>
#include <algorithm>
#include <chrono>

CalorieTracker::CalorieTracker() 
    : dataManager(std::make_unique<DataManager>("./data")) {}

CalorieTracker& CalorieTracker::getInstance() {
    static CalorieTracker instance;
    return instance;
}

CalorieTracker::~CalorieTracker() {
    try {
        if (!currentUsername.empty()) saveData();
    } catch (...) {}
}

UserProfile* CalorieTracker::getUserProfile() const noexcept {
    return user.get();
}

const std::vector<std::unique_ptr<Meal>>& CalorieTracker::getMeals() const noexcept {
    return meals;
}

const std::vector<std::shared_ptr<FoodItem>>& CalorieTracker::getAllFoods() const noexcept {
    return foodDatabase;
}

std::string CalorieTracker::getCurrentUsername() const noexcept {
    return currentUsername;
}
