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

void CalorieTracker::initialize(std::string username) {
    currentUsername = username;
    user.reset();
    meals.clear();
    foodDatabase.clear();

    auto loadedUser = dataManager->loadUserProfile(username);
    if (loadedUser.has_value()) {
        user = std::make_unique<UserProfile>(std::move(loadedUser.value()));
        std::cout << "✅ Profile loaded for: " << username << std::endl;
    } else {
        std::cout << "👤 New user: " << username << std::endl;
    }

    auto loadedMeals = dataManager->loadMeals(username);
    if (loadedMeals.has_value()) meals = std::move(loadedMeals.value());

    auto loadedFoods = dataManager->loadFoodDatabase(username);
    if (loadedFoods.has_value()) foodDatabase = std::move(loadedFoods.value());
}

bool CalorieTracker::loadData() {
    if (currentUsername.empty()) return false;
    meals.clear();
    foodDatabase.clear();
    auto loadedMeals = dataManager->loadMeals(currentUsername);
    if (loadedMeals.has_value()) meals = std::move(loadedMeals.value());
    auto loadedFoods = dataManager->loadFoodDatabase(currentUsername);
    if (loadedFoods.has_value()) foodDatabase = std::move(loadedFoods.value());
    return true;
}

void CalorieTracker::saveData() {
    if (currentUsername.empty()) return;
    if (user) dataManager->saveUserProfile(*user);
    dataManager->saveMeals(meals, currentUsername);
    dataManager->saveFoodDatabase(foodDatabase, currentUsername);
    std::cout << "💾 Data saved for user: " << currentUsername << std::endl;
}
