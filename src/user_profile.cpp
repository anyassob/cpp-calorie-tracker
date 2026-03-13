#include "../include/user_profile.h"
#include <cmath>
#include "exceptions.h"

UserProfile::UserProfile(std::string name, double weight, double height, int age, char gender)
        : name(std::move(name))
        , weight(weight)
        , height(height)
        , age(age)
        , gender(gender) {

    if (weight <= 0 || weight > 500) {
        throw ValidationException("Weight must be between 0 and 500 kg");
    }
    if (height <= 0 || height > 300) {
        throw ValidationException("Height must be between 0 and 300 cm");
    }
    if (age <= 0 || age > 150) {
        throw ValidationException("Age must be between 0 and 150 years");
    }
    if (gender != 'M' && gender != 'F') {
        throw ValidationException("Gender must be 'M' or 'F'");
    }
}


double UserProfile::calculateBMR() const noexcept {
    double bmr = (10 * weight) + (6.25 * height) - (5 * age);
    return (gender == 'M') ? bmr + 5 : bmr - 161;
}

std::string UserProfile::getName() const noexcept {
    return name;
}

double UserProfile::getWeight() const noexcept {
    return weight;
}

double UserProfile::getHeight() const noexcept {
    return height;
}

int UserProfile::getAge() const noexcept {
    return age;
}

char UserProfile::getGender() const noexcept {
    return gender;
}

void UserProfile::setDailyCalorieGoal(double goal) {
    if (goal <= 0) {
        throw ValidationException("Calorie goal must be positive");
    }
    dailyCalorieGoal = goal;
}

std::optional<double> UserProfile::getDailyCalorieGoal() const noexcept {
    return dailyCalorieGoal;
}

void UserProfile::updateWeight(double newWeight) {
    if (newWeight <= 0 || newWeight > 500) {
        throw ValidationException("Weight must be between 0 and 500 kg");
    }
    weight = newWeight;
}

void UserProfile::updateHeight(double newHeight) {
    if (newHeight <= 0 || newHeight > 300) {
        throw ValidationException("Height must be between 0 and 300 cm");
    }
    height = newHeight;
}