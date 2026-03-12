#include "../include/exceptions.h"
#include "../include/basic_food.h"

BasicFood::BasicFood(std::string name, double protein, double fat, double carbs, double portionMultiplier)
    : name(std::move(name))
    , protein(protein)
    , fat(fat)
    , carbs(carbs)
    , portionMultiplier(portionMultiplier) {}

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
