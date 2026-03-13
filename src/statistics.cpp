#include "../include/statistics.h"
#include <algorithm>
#include <map>

Statistics::Statistics(const std::vector<std::unique_ptr<Meal>>& mealRefs) {
    for (const auto& meal : mealRefs) {
        meals.emplace_back(*meal);
    }
}

Statistics::DailySummary Statistics::getDailySummary(std::chrono::system_clock::time_point day) const {
    auto dayStart = std::chrono::floor<std::chrono::days>(day);
    auto dayEnd = dayStart + std::chrono::hours(24);

    DailySummary summary{0.0, 0.0, 0.0, 0.0, 0};

    for (const auto& mealRef : meals) {
        auto mealTime = mealRef.get().getTime();
        if (mealTime >= dayStart && mealTime < dayEnd) {
            summary.totalCalories += mealRef.get().getTotalCalories();
            summary.totalProtein += mealRef.get().getTotalProtein();
            summary.totalFat += mealRef.get().getTotalFat();
            summary.totalCarbs += mealRef.get().getTotalCarbs();
            summary.mealCount++;
        }
    }

    return summary;
}

std::optional<std::reference_wrapper<const Meal>> Statistics::getLargestMeal() const {
    if (meals.empty()) {
        return std::nullopt;
    }

    auto largest = std::max_element(meals.begin(), meals.end(),
                                    [](const auto& a, const auto& b) {
                                        return a.get().getTotalCalories() < b.get().getTotalCalories();
                                    });

    return std::ref(largest->get());
}

std::map<std::chrono::system_clock::time_point, Statistics::DailySummary>
Statistics::getWeeklySummary(std::chrono::system_clock::time_point startDate) const {

    std::map<std::chrono::system_clock::time_point, DailySummary> weeklyData;

    for (int i = 0; i < 7; ++i) {
        auto day = startDate + std::chrono::hours(24 * i);
        weeklyData[day] = getDailySummary(day);
    }

    return weeklyData;
}
std::optional<double> Statistics::getProgressToGoal(double dailyGoal,
                                                    std::chrono::system_clock::time_point day) const {
    if (dailyGoal <= 0) {
        return std::nullopt;
    }

    auto summary = getDailySummary(day);
    if (summary.mealCount == 0) {
        return 0.0;
    }

    return (summary.totalCalories / dailyGoal) * 100.0;
}