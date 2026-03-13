#pragma once

#include <string>
#include <memory>
#include <optional>
#include <string_view>

class FoodItem {
public:
    virtual ~FoodItem() = default;

    virtual std::string getName() const noexcept = 0;
    virtual double getCalories() const noexcept = 0;
    virtual double getProtein() const noexcept = 0;
    virtual double getFat() const noexcept = 0;
    virtual double getCarbs() const noexcept = 0;
    virtual std::string getDescription() const = 0;
    virtual void updatePortion(double multiplier) = 0;
    virtual std::unique_ptr<FoodItem> clone() const = 0;

    static constexpr double caloriesPerGram(double protein, double fat, double carbs) noexcept {
        return (protein * 4.0) + (fat * 9.0) + (carbs * 4.0);
    }

    virtual std::optional<double> getNutrient(std::string_view nutrientName) const noexcept;
};
