#pragma once
#include "food_item.h"
#include <vector>
#include <memory>
#include <chrono>
#include <string>

class Meal {
private:
    std::string name;
    std::chrono::system_clock::time_point time;
    std::vector<std::unique_ptr<FoodItem>> items;
public:
    explicit Meal(std::string name);

    Meal(const Meal&) = delete;
    Meal& operator=(const Meal&) = delete;
    Meal(Meal&&) noexcept = default;
    Meal& operator=(Meal&&) noexcept = default;

    void addFood(std::unique_ptr<FoodItem> food);
    [[nodiscard]] std::string getName() const noexcept;
    [[nodiscard]] std::chrono::system_clock::time_point getTime() const noexcept;
    [[nodiscard]] double getTotalCalories() const noexcept;
    [[nodiscard]] double getTotalProtein() const noexcept;
    [[nodiscard]] double getTotalFat() const noexcept;
    [[nodiscard]] double getTotalCarbs() const noexcept;

    ~Meal() = default;
};
