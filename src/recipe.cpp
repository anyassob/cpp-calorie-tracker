#include "recipe.h"
#include "exceptions.h"
#include <numeric>
#include <sstream>
#include <iomanip>

Recipe::Recipe(std::string name) : name(std::move(name)), totalWeight(0.0) {}

std::string Recipe::getName() const noexcept { return name; }

void Recipe::addIngredient(std::shared_ptr<FoodItem> ingredient, double weightInGrams) {
    if (!ingredient) throw ValidationException("Ingredient cannot be null");
    if (weightInGrams <= 0) throw ValidationException("Weight must be positive");
    ingredients.emplace_back(ingredient, weightInGrams);
    totalWeight += weightInGrams;
}

double Recipe::getCalories() const noexcept {
    double total = 0.0;
    for (const auto& [food, weight] : ingredients)
        total += food->getCalories() * weight / 100.0;
    return total;
}

double Recipe::getProtein() const noexcept {
    double total = 0.0;
    for (const auto& [food, weight] : ingredients)
        total += food->getProtein() * weight / 100.0;
    return total;
}

double Recipe::getFat() const noexcept {
    double total = 0.0;
    for (const auto& [food, weight] : ingredients)
        total += food->getFat() * weight / 100.0;
    return total;
}

double Recipe::getCarbs() const noexcept {
    double total = 0.0;
    for (const auto& [food, weight] : ingredients)
        total += food->getCarbs() * weight / 100.0;
    return total;
}

std::string Recipe::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "Recipe: " << name << "\\n";
    oss << "Weight: " << totalWeight << "g\\n";
    oss << "Calories: " << getCalories() << " kcal";
    return oss.str();
}

void Recipe::updatePortion(double multiplier) {
    if (multiplier <= 0) throw ValidationException("Multiplier must be positive");
    for (auto& [food, weight] : ingredients) weight *= multiplier;
    totalWeight *= multiplier;
}

std::unique_ptr<FoodItem> Recipe::clone() const {
    auto clone = std::make_unique<Recipe>(name);
    clone->ingredients = ingredients;
    clone->totalWeight = totalWeight;
    return clone;
}

size_t Recipe::getIngredientCount() const { return ingredients.size(); }
double Recipe::getTotalWeight() const { return totalWeight; }
