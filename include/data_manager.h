#pragma once

#include "user_profile.h"
#include "meal.h"
#include "basic_food.h"
#include "recipe.h"
#include "exceptions.h"
#include <memory>
#include <vector>
#include <optional>
#include <string>

class DataManager {
private:
    std::string dataDirectory;
    
    bool saveToFile(const std::string& filename, const std::string& content);
    std::optional<std::string> loadFromFile(const std::string& filename);
    
public:
    explicit DataManager(std::string directory);
    
    void saveUserProfile(const UserProfile& profile);
    std::optional<UserProfile> loadUserProfile(const std::string& username);
    
    void saveMeals(const std::vector<std::unique_ptr<Meal>>& meals, const std::string& username);
    std::optional<std::vector<std::unique_ptr<Meal>>> loadMeals(const std::string& username);
    
    void saveFoodDatabase(const std::vector<std::shared_ptr<FoodItem>>& foods, const std::string& username);
    std::optional<std::vector<std::shared_ptr<FoodItem>>> loadFoodDatabase(const std::string& username);
    
    bool dataExists(const std::string& key) const;
};
