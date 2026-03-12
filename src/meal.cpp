#include "meal.h"
#include "exceptions.h"
#include <numeric>

Meal::Meal(std::string name)
    : name(std::move(name))
    , time(std::chrono::system_clock::now()) {}

std::string Meal::getName() const noexcept {
    return name;
}

std::chrono::system_clock::time_point Meal::getTime() const noexcept {
    return time;
}
