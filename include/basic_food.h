#pragma once

#include "food_item.h"
#include "exceptions.h"
#include <memory>

class BasicFood : public FoodItem {
private:
    std::string name;
    double protein;
    double fat;
    double carbs;
    double portionMultiplier;

public:
    BasicFood(std::string name, double protein, double fat, double carbs, double portionMultiplier = 1.0);

    std::string getName() const noexcept override;
    double getCalories() const noexcept override;
    double getProtein() const noexcept override;
    double getFat() const noexcept override;
    double getCarbs() const noexcept override;
    std::string getDescription() const override;
    void updatePortion(double multiplier) override;
    std::unique_ptr<FoodItem> clone() const override;

    double getPortionMultiplier() const noexcept;
    void setPortionMultiplier(double multiplier);
};
