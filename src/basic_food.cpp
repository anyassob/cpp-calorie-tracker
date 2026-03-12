#include "../include/exceptions.h"
#include "../include/basic_food.h"
#include <sstream>
#include <iomanip>

BasicFood::BasicFood(std::string name, double protein, double fat, double carbs, double portionMultiplier)
    : name(std::move(name))
    , protein(protein)
    , fat(fat)
    , carbs(carbs)
, portionMultiplier(portionMultiplier) { if (protein < 0 || fat < 0 || carbs < 0 || portionMultiplier <= 0) {
    throw ValidationException("Nutrient values and portion multiplier must be positive");
}}

std::string BasicFood::getName() const noexcept {
    return name;
}

double BasicFood::getProtein() const noexcept {
    return protein * portionMultiplier;
}

double BasicFood::getFat() const noexcept {
    return fat * portionMultiplier;
}

double BasicFood::getCarbs() const noexcept {
    return carbs * portionMultiplier;
}

double BasicFood::getCalories() const noexcept {
    return (protein * 4.0 + fat * 9.0 + carbs * 4.0) * portionMultiplier;
}

std::string BasicFood::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << name << " ("
        << (protein * portionMultiplier) << "g protein, "
        << (fat * portionMultiplier) << "g fat, "
        << (carbs * portionMultiplier) << "g carbs, "
        << getCalories() << " kcal)";
    return oss.str();
}

void BasicFood::updatePortion(double multiplier) {
    if (multiplier <= 0) {
        throw ValidationException("Portion multiplier must be positive");
    }
    portionMultiplier *= multiplier;
}

std::unique_ptr<FoodItem> BasicFood::clone() const {
    return std::make_unique<BasicFood>(*this);
}
