
#pragma once

#include "food_item.h"
#include "exceptions.h"
#include <vector>
#include <memory>
#include <string>

class Recipe : public FoodItem {
private:
    std::string name;
    std::vector<std::pair<std::shared_ptr<FoodItem>, double>> ingredients;
    double totalWeight;

public:
    explicit Recipe(std::string name);
    std::unique_ptr<FoodItem> clone() const override;
    void addIngredient(std::shared_ptr<FoodItem> ingredient, double weightInGrams);

    std::string getName() const noexcept override;
    double getCalories() const noexcept override;
    double getProtein() const noexcept override;
    double getFat() const noexcept override;
    double getCarbs() const noexcept override;
    std::string getDescription() const override;

    void updatePortion(double multiplier) override;
    const std::vector<std::pair<std::shared_ptr<FoodItem>, double>>& getIngredients() const noexcept;
    double getNutrientPer100g(const std::string& nutrient) const;
    size_t getIngredientCount() const noexcept;
    double getTotalWeight() const noexcept;
};
