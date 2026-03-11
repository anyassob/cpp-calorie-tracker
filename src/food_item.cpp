#include "food_item.h"

std::optional<double> FoodItem::getNutrient(std::string_view nutrientName) const noexcept {
    if (nutrientName == "calories") return getCalories();
    if (nutrientName == "protein") return getProtein();
    if (nutrientName == "fat") return getFat();
    if (nutrientName == "carbs") return getCarbs();
    return std::nullopt;
}
