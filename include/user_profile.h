#pragma once

#include <string>
#include <optional>

class UserProfile {
private:
    std::string name;
    double weight;
    double height;
    int age;
    char gender;
    std::optional<double> dailyCalorieGoal;

public:
    UserProfile(std::string name, double weight, double height, int age, char gender);

    [[nodiscard]] double calculateBMR() const noexcept;

    [[nodiscard]] std::string getName() const noexcept;
    [[nodiscard]] double getWeight() const noexcept;
    [[nodiscard]] double getHeight() const noexcept;
    [[nodiscard]] int getAge() const noexcept;
    [[nodiscard]] char getGender() const noexcept;

    void setDailyCalorieGoal(double goal);
    [[nodiscard]] std::optional<double> getDailyCalorieGoal() const noexcept;

    void updateWeight(double newWeight);
    void updateHeight(double newHeight);
};