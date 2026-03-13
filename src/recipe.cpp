#include "recipe.h"
#include "exceptions.h"
#include <numeric>
#include <sstream>
#include <iomanip>

Recipe::Recipe(std::string name)
    : name(std::move(name))
    , totalWeight(0.0) {}

std::string Recipe::getName() const noexcept {
    return name;
}

void Recipe::addIngredient(std::shared_ptr<FoodItem> ingredient, double weightInGrams) {
    if (!ingredient) {
        throw ValidationException("Ingredient cannot be null");
    }
    if (weightInGrams <= 0) {
        throw ValidationException("Weight must be positive");
    }
    ingredients.emplace_back(std::move(ingredient), weightInGrams);
    totalWeight += weightInGrams;
}

double Recipe::getCalories() const noexcept {
    return std::accumulate(ingredients.begin(), ingredients.end(), 0.0,
        [](double sum, const auto& item) {
            const auto& [food, weight] = item;
            return sum + (food->getCalories() * weight / 100.0);
        });
}

double Recipe::getProtein() const noexcept {
    return std::accumulate(ingredients.begin(), ingredients.end(), 0.0,
        [](double sum, const auto& item) {
            const auto& [food, weight] = item;
            return sum + (food->getProtein() * weight / 100.0);
        });
}

double Recipe::getFat() const noexcept {
    return std::accumulate(ingredients.begin(), ingredients.end(), 0.0,
        [](double sum, const auto& item) {
            const auto& [food, weight] = item;
            return sum + (food->getFat() * weight / 100.0);
        });
}

double Recipe::getCarbs() const noexcept {
    return std::accumulate(ingredients.begin(), ingredients.end(), 0.0,
        [](double sum, const auto& item) {
            const auto& [food, weight] = item;
            return sum + (food->getCarbs() * weight / 100.0);
        });
}

std::string Recipe::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "Recipe: " << name << "\n";
    oss << "Total weight: " << totalWeight << "g\n";
    oss << "Calories: " << getCalories() << " kcal\n";
    oss << "Protein: " << getProtein() << "g, Fat: " << getFat() << "g, Carbs: " << getCarbs() << "g\n";
    oss << "Ingredients:\n";
    
    for (const auto& [food, weight] : ingredients) {
        oss << "  - " << food->getName() << ": " << weight << "g\n";
    }
    
    return oss.str();
}

void Recipe::updatePortion(double multiplier) {
    if (multiplier <= 0) {
        throw ValidationException("Portion multiplier must be positive");
    }
    for (auto& [food, weight] : ingredients) {
        weight *= multiplier;
    }
    totalWeight *= multiplier;
}

std::unique_ptr<FoodItem> Recipe::clone() const {
    auto newRecipe = std::make_unique<Recipe>(name);
    newRecipe->ingredients = ingredients;
    newRecipe->totalWeight = totalWeight;
    return newRecipe;
}

size_t Recipe::getIngredientCount() const noexcept {
    return ingredients.size();
}

double Recipe::getTotalWeight() const noexcept {
    return totalWeight;
}

const std::vector<std::pair<std::shared_ptr<FoodItem>, double>>& Recipe::getIngredients() const noexcept {
    return ingredients;
}
