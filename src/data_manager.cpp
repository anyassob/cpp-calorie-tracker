#include "data_manager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

DataManager::DataManager(std::string directory) 
    : dataDirectory(std::move(directory)) {
    
    try {
        if (!fs::exists(dataDirectory)) {
            fs::create_directories(dataDirectory);
        }
    } catch (const fs::filesystem_error& e) {
        throw FileIOException(dataDirectory);
    }
}

bool DataManager::saveToFile(const std::string& filename, const std::string& content) {
    std::string fullPath = dataDirectory + "/" + filename;
    
    try {
        std::ofstream file(fullPath);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return file.good();
    } catch (const std::exception& e) {
        throw FileIOException(filename);
    }
}

std::optional<std::string> DataManager::loadFromFile(const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    
    try {
        if (!fs::exists(fullPath)) {
            return std::nullopt;
        }
        
        std::ifstream file(fullPath);
        if (!file.is_open()) {
            return std::nullopt;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } catch (const std::exception& e) {
        throw FileIOException(filename);
    }
}

void DataManager::saveUserProfile(const UserProfile& profile) {
    std::ostringstream oss;
    oss << profile.getName() << "\n"
        << profile.getWeight() << "\n"
        << profile.getHeight() << "\n"
        << profile.getAge() << "\n"
        << profile.getGender() << "\n";
    
    auto goal = profile.getDailyCalorieGoal();
    if (goal.has_value()) {
        oss << goal.value();
    } else {
        oss << "none";
    }
    
    if (!saveToFile("user_" + profile.getName() + ".txt", oss.str())) {
        throw FileIOException("user_" + profile.getName() + ".txt");
    }
}

std::optional<UserProfile> DataManager::loadUserProfile(const std::string& username) {
    auto content = loadFromFile("user_" + username + ".txt");
    if (!content.has_value()) {
        return std::nullopt;
    }
    
    std::istringstream iss(content.value());
    std::string name;
    double weight, height;
    int age;
    char gender;
    std::string goalStr;
    
    std::getline(iss, name);
    iss >> weight >> height >> age >> gender;
    iss.ignore();
    std::getline(iss, goalStr);
    
    UserProfile profile(name, weight, height, age, gender);
    
    if (goalStr != "none") {
        profile.setDailyCalorieGoal(std::stod(goalStr));
    }
    
    return profile;
}

void DataManager::saveMeals(const std::vector<std::unique_ptr<Meal>>& meals, const std::string& username) {
    std::ostringstream oss;
    oss << meals.size() << "\n";
    for (const auto& meal : meals) {
        oss << meal->getName() << "\n";
    }
    
    if (!saveToFile(username + "_meals.txt", oss.str())) {
        throw FileIOException(username + "_meals.txt");
    }
}

std::optional<std::vector<std::unique_ptr<Meal>>> DataManager::loadMeals(const std::string& username) {
    auto content = loadFromFile(username + "_meals.txt");
    if (!content.has_value()) {
        return std::nullopt;
    }
    
    std::vector<std::unique_ptr<Meal>> meals;
    std::istringstream iss(content.value());
    size_t count;
    iss >> count;
    iss.ignore();
    
    for (size_t i = 0; i < count; ++i) {
        std::string mealName;
        std::getline(iss, mealName);
        meals.push_back(std::make_unique<Meal>(mealName));
    }
    
    return meals;
}

void DataManager::saveFoodDatabase(const std::vector<std::shared_ptr<FoodItem>>& foods, const std::string& username) {
    std::ostringstream oss;
    oss << foods.size() << "\n";
    
    for (const auto& food : foods) {
        if (auto basicFood = std::dynamic_pointer_cast<BasicFood>(food)) {
            oss << "BASIC\n";
            oss << basicFood->getName() << "\n";
            oss << basicFood->getProtein() << " "
                << basicFood->getFat() << " "
                << basicFood->getCarbs() << " "
                << "1.0\n";
        }
    }
    
    if (!saveToFile(username + "_foods.txt", oss.str())) {
        throw FileIOException(username + "_foods.txt");
    }
}
