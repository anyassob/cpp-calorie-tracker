#pragma once

#include <stdexcept>
#include <string>

class CalorieTrackerException : public std::runtime_error {
public:
    explicit CalorieTrackerException(const std::string& message) 
        : std::runtime_error(message) {}
};

class FileIOException : public CalorieTrackerException {
public:
    explicit FileIOException(const std::string& filename)
        : CalorieTrackerException("File error: " + filename) {}
};

class ValidationException : public CalorieTrackerException {
public:
    explicit ValidationException(const std::string& reason)
        : CalorieTrackerException("Validation error: " + reason) {}
};
