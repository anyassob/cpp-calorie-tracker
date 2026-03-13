#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <map>
#include <optional>
#include <functional>

class Statistics {
private:
    std::vector<std::reference_wrapper<const Meal>> meals;

public:
    explicit Statistics(const std::vector<std::unique_ptr<Meal>>& mealRefs);

    struct DailySummary {
        double totalCalories;
        double totalProtein;
        double totalFat;
        double totalCarbs;
        size_t mealCount;
    };

    [[nodiscard]] DailySummary getDailySummary(std::chrono::system_clock::time_point day) const;

    [[nodiscard]] std::optional<std::reference_wrapper<const Meal>> getLargestMeal() const;

    [[nodiscard]] std::map<std::chrono::system_clock::time_point, DailySummary>
    getWeeklySummary(std::chrono::system_clock::time_point startDate) const;

    [[nodiscard]] std::optional<double> getProgressToGoal(double dailyGoal,
                                                          std::chrono::system_clock::time_point day) const;
};