#include "../include/calorie_tracker.h"
#include "../include/basic_food.h"
#include "../include/recipe.h"
#include "../include/meal.h"
#include "../include/user_profile.h"
#include "../include/exceptions.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <limits>
#include <cctype>
#include <sstream>

const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string BLUE    = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN    = "\033[36m";
const std::string BOLD    = "\033[1m";

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printSuccess(const std::string& msg) {
    std::cout << GREEN << "✓ " << msg << RESET << std::endl;
}

void printError(const std::string& msg) {
    std::cout << RED << "✗ " << msg << RESET << std::endl;
}

void printWarning(const std::string& msg) {
    std::cout << YELLOW << "⚠ " << msg << RESET << std::endl;
}

void printTitle(const std::string& title) {
    std::cout << BOLD << CYAN << "\n--- " << title << " ---" << RESET << std::endl;
}

void printWelcome() {
    clearScreen();
    std::cout << BOLD << MAGENTA;
    std::cout << R"(
╔══════════════════════════════════════════════╗
║         🍏 CALORIE TRACKER 🍏                  ║
║     Track your health with style             ║
╚══════════════════════════════════════════════╝
)" << RESET << std::endl;
}

void printMenu() {
    std::cout << BOLD << CYAN << "\n╔════════════════════════════════════╗\n";
    std::cout << "║         MAIN MENU                  ║\n";
    std::cout << "╚════════════════════════════════════╝\n" << RESET;
    std::cout << "  " << GREEN << "1." << RESET << " Add new product\n";
    std::cout << "  " << GREEN << "2." << RESET << " Show all products\n";
    std::cout << "  " << GREEN << "3." << RESET << " Add a meal\n";
    std::cout << "  " << GREEN << "4." << RESET << " Show today's statistics\n";
    std::cout << "  " << GREEN << "5." << RESET << " Create a recipe\n";
    std::cout << "  " << GREEN << "6." << RESET << " Show all recipes\n";
    std::cout << "  " << YELLOW << "7." << RESET << " Set up profile (with calorie norm calculation)\n";
    std::cout << "  " << CYAN << "8." << RESET << " Show profile\n";
    std::cout << "  " << RED << "0." << RESET << " Exit\n";
    std::cout << BOLD << "Choose an option: " << RESET;
}

void setupProfile(CalorieTracker& tracker, const std::string& username) {
    printTitle("PROFILE SETUP & CALORIE CALCULATION");

    double weight, height;
    int age;
    char gender;

    std::cout << "Name: " << username << "\n";

    std::cout << "Enter your weight (kg): ";
    std::cin >> weight;

    std::cout << "Enter your height (cm): ";
    std::cin >> height;

    std::cout << "Enter your age (years): ";
    std::cin >> age;

    std::cout << "Enter your gender (M/F): ";
    std::cin >> gender;

    std::cout << "\nSelect your activity level:\n";
    std::cout << "1. Sedentary (little or no exercise)\n";
    std::cout << "2. Lightly active (light exercise 1-3 days/week)\n";
    std::cout << "3. Moderately active (moderate exercise 3-5 days/week)\n";
    std::cout << "4. Very active (hard exercise 6-7 days/week)\n";
    std::cout << "5. Super active (physical job + training)\n";

    int activityChoice;
    double activityMultiplier;
    std::cout << "Your choice (1-5): ";
    std::cin >> activityChoice;

    switch(activityChoice) {
        case 1: activityMultiplier = 1.2; break;
        case 2: activityMultiplier = 1.375; break;
        case 3: activityMultiplier = 1.55; break;
        case 4: activityMultiplier = 1.725; break;
        case 5: activityMultiplier = 1.9; break;
        default:
            printWarning("Invalid choice, using medium activity (1.55)");
            activityMultiplier = 1.55;
    }

    double bmr;
    if (gender == 'M' || gender == 'm') {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) + 5;
    } else {
        bmr = (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }

    double dailyNorm = bmr * activityMultiplier;

    std::cout << "\n--- CALCULATION RESULT ---\n";
    std::cout << "Your basal metabolic rate: " << std::fixed << std::setprecision(0) << bmr << " kcal/day\n";
    std::cout << "Activity multiplier: " << activityMultiplier << "\n";
    std::cout << "Daily calorie norm: " << dailyNorm << " kcal/day\n";

    try {
        auto user = std::make_unique<UserProfile>(username, weight, height, age, gender);
        user->setDailyCalorieGoal(dailyNorm);
        tracker.setUserProfile(std::move(user));
        tracker.saveData();

        printSuccess("PROFILE CREATED SUCCESSFULLY!");
        std::cout << "  Name: " << username << "\n";
        std::cout << "  Weight: " << weight << " kg\n";
        std::cout << "  Height: " << height << " cm\n";
        std::cout << "  Age: " << age << " years\n";
        std::cout << "  Gender: " << gender << "\n";
        std::cout << "  Daily norm: " << dailyNorm << " kcal\n";

    } catch (const ValidationException& e) {
        printError(e.what());
    }
}

void showProfile(CalorieTracker& tracker) {
    printTitle("USER PROFILE");

    if (auto user = tracker.getUserProfile()) {
        std::cout << "Name: " << user->getName() << "\n";
        std::cout << "Weight: " << user->getWeight() << " kg\n";
        std::cout << "Height: " << user->getHeight() << " cm\n";
        std::cout << "Age: " << user->getAge() << " years\n";
        std::cout << "Gender: " << user->getGender() << "\n";

        if (auto goal = user->getDailyCalorieGoal()) {
            std::cout << "Daily norm: " << goal.value() << " kcal\n";
        } else {
            printWarning("Daily norm not set.");
        }
    } else {
        printWarning("Profile not set up. Use option 7.");
    }
}

void addNewProduct(CalorieTracker& tracker) {
    printTitle("ADD NEW PRODUCT");

    std::string name;
    double protein, fat, carbs;

    std::cout << "Product name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::cout << "Protein per 100g (g): ";
    std::cin >> protein;

    std::cout << "Fat per 100g (g): ";
    std::cin >> fat;

    std::cout << "Carbs per 100g (g): ";
    std::cin >> carbs;

    try {
        auto food = std::make_shared<BasicFood>(name, protein, fat, carbs, 1.0);
        tracker.addToFoodDatabase(food);
        tracker.saveData();

        double calories = food->getCalories();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "Product \"" << name << "\" added! Calories: " << calories << " kcal/100g";
        printSuccess(oss.str());

    } catch (const ValidationException& e) {
        printError(e.what());
    }
}

void showAllProducts(CalorieTracker& tracker) {
    printTitle("ALL PRODUCTS");

    const auto& foods = tracker.getAllFoods();
    std::vector<std::shared_ptr<BasicFood>> basicFoods;

    for (const auto& food : foods) {
        if (auto basic = std::dynamic_pointer_cast<BasicFood>(food)) {
            basicFoods.push_back(basic);
        }
    }

    if (basicFoods.empty()) {
        printWarning("No products found (only recipes exist).");
        return;
    }

    const int noWidth = 2;
    const int nameWidth = 20;
    const int numWidth = 8;
    const int calWidth = 10;

    std::cout << "┌────┬──────────────────────┬──────────┬──────────┬──────────┬────────────┐\n";
    std::cout << "│ " << BOLD << std::left << std::setw(noWidth) << "No." << RESET << " │ "
              << BOLD << std::left << std::setw(nameWidth) << "Name" << RESET << " │ "
              << BOLD << std::right << std::setw(numWidth) << "Protein" << RESET << " │ "
              << BOLD << std::right << std::setw(numWidth) << "Fat" << RESET << " │ "
              << BOLD << std::right << std::setw(numWidth) << "Carbs" << RESET << " │ "
              << BOLD << std::right << std::setw(calWidth) << "kcal/100g" << RESET << " │\n";
    std::cout << "├────┼──────────────────────┼──────────┼──────────┼──────────┼────────────┤\n";

    int index = 1;
    for (const auto& food : basicFoods) {
        std::string name = food->getName();
        if (name.length() > nameWidth) {
            name = name.substr(0, nameWidth - 3) + "...";
        }

        std::cout << "│ " << std::right << std::setw(noWidth) << index++ << " │ "
                  << std::left << std::setw(nameWidth) << name << " │ "
                  << std::right << std::setw(numWidth) << std::fixed << std::setprecision(1) << food->getProtein() << " │ "
                  << std::right << std::setw(numWidth) << food->getFat() << " │ "
                  << std::right << std::setw(numWidth) << food->getCarbs() << " │ "
                  << std::right << std::setw(calWidth) << food->getCalories() << " │\n";
    }

    std::cout << "└────┴──────────────────────┴──────────┴──────────┴──────────┴────────────┘\n";
    printSuccess("Total products: " + std::to_string(basicFoods.size()));
}

void addNewMeal(CalorieTracker& tracker) {
    printTitle("ADD A MEAL");

    const auto& foods = tracker.getAllFoods();
    if (foods.empty()) {
        printError("No products found. Add products first.");
        return;
    }

    std::string mealName;
    std::cout << "Meal name: ";
    std::cin.ignore();
    std::getline(std::cin, mealName);

    auto meal = std::make_unique<Meal>(mealName);

    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        std::cout << "\n--- Available products ---\n";
        for (size_t i = 0; i < foods.size(); ++i) {
            std::cout << i + 1 << ". " << foods[i]->getName()
                      << " - " << std::fixed << std::setprecision(1)
                      << foods[i]->getCalories() << " kcal/100g\n";
        }

        std::cout << "\nChoose product number: ";
        int choice;
        std::cin >> choice;

        if (choice < 1 || choice > static_cast<int>(foods.size())) {
            printError("Invalid number!");
            continue;
        }

        auto selectedFood = foods[choice - 1];

        double grams;
        std::cout << "Weight (grams): ";
        std::cin >> grams;

        auto foodCopy = selectedFood->clone();
        foodCopy->updatePortion(grams / 100.0);
        meal->addFood(std::move(foodCopy));

        double calories = selectedFood->getCalories() * grams / 100.0;
        std::cout << GREEN << "✓ Added: " << grams << "g ("
                  << std::fixed << std::setprecision(1) << calories << " kcal)" << RESET << "\n";

        std::cout << "\nAdd another? (y/n): ";
        std::cin >> addMore;
    }

    if (meal->getTotalCalories() > 0) {
        tracker.addMeal(std::move(meal));
        tracker.saveData();
        printSuccess("Meal added!");
    } else {
        printWarning("Meal is empty, not added.");
    }
}

void showStatistics(CalorieTracker& tracker) {
    printTitle("TODAY'S STATISTICS");

    auto summary = tracker.getTodaysSummary();
    if (!summary.has_value()) {
        printWarning("No data for today. Add some meals first.");
        return;
    }

    std::cout << std::fixed << std::setprecision(1);

    std::cout << "┌────────────────────────────┬─────────────┐\n";
    std::cout << "│ " << BOLD << std::left << std::setw(26) << "Indicator" << RESET << " │ "
              << BOLD << std::right << std::setw(11) << "Value" << RESET << " │\n";
    std::cout << "├────────────────────────────┼─────────────┤\n";

    std::cout << "│ " << std::left << std::setw(26) << "Calories" << " │ "
              << std::right << std::setw(8) << summary->totalCalories << " kcal   │\n";

    std::cout << "│ " << std::left << std::setw(26) << "Protein" << " │ "
              << std::right << std::setw(8) << summary->totalProtein << " g      │\n";

    std::cout << "│ " << std::left << std::setw(26) << "Fat" << " │ "
              << std::right << std::setw(8) << summary->totalFat << " g      │\n";

    std::cout << "│ " << std::left << std::setw(26) << "Carbohydrates" << " │ "
              << std::right << std::setw(8) << summary->totalCarbs << " g      │\n";

    std::cout << "│ " << std::left << std::setw(26) << "Number of meals" << " │ "
              << std::right << std::setw(8) << summary->mealCount << "        │\n";

    std::cout << "└────────────────────────────┴─────────────┘\n";

    if (auto user = tracker.getUserProfile()) {
        if (auto goal = user->getDailyCalorieGoal()) {
            double remaining = goal.value() - summary->totalCalories;

            std::cout << "\n" << BOLD << "Daily goal: " << RESET << goal.value() << " kcal\n";

            if (remaining > 0) {
                std::cout << GREEN << "✓ Remaining: " << remaining << " kcal" << RESET << "\n";

                double percent = (summary->totalCalories / goal.value()) * 100.0;
                int barLength = 30;
                int filled = static_cast<int>(percent / 100.0 * barLength);

                std::cout << "Progress: [";
                for (int i = 0; i < barLength; ++i) {
                    if (i < filled) std::cout << GREEN << "#" << RESET;
                    else std::cout << "-";
                }
                std::cout << "] " << std::fixed << std::setprecision(1) << percent << "%\n";

            } else if (remaining < 0) {
                std::cout << RED << "✗ Exceeded by: " << -remaining << " kcal" << RESET << "\n";

                std::cout << "Progress: [" << GREEN << std::string(30, '#') << RESET << "] 100.0%\n";

            } else {
                std::cout << GREEN << "✓ Perfect! Exactly at goal!" << RESET << "\n";
            }
        }
    }

    std::cout << "\n" << BOLD << "MEAL DETAILS:" << RESET << "\n";
    const auto& meals = tracker.getMeals();
    if (!meals.empty()) {
        int mealNumber = 1;
        for (const auto& meal : meals) {
            auto mealTime = std::chrono::system_clock::to_time_t(meal->getTime());
            std::string timeStr = std::ctime(&mealTime);
            timeStr = timeStr.substr(0, timeStr.length() - 1);

            std::cout << "\n" << CYAN << "  " << mealNumber++ << ". " << meal->getName() << RESET << "\n";
            std::cout << "     Time: " << timeStr << "\n";
            std::cout << "     Calories: " << BOLD << meal->getTotalCalories() << RESET << " kcal | "
                      << "P: " << meal->getTotalProtein() << "g | "
                      << "F: " << meal->getTotalFat() << "g | "
                      << "C: " << meal->getTotalCarbs() << "g\n";
        }

        std::cout << "\n" << BOLD << "─────────────────────────────────────" << RESET << "\n";
        std::cout << "  TOTAL: " << BOLD << summary->totalCalories << RESET << " kcal"
                  << " | P: " << summary->totalProtein << "g"
                  << " | F: " << summary->totalFat << "g"
                  << " | C: " << summary->totalCarbs << "g\n";

    } else {
        std::cout << "  No meals today.\n";
    }
}

void createRecipe(CalorieTracker& tracker) {
    printTitle("CREATE RECIPE");

    const auto& foods = tracker.getAllFoods();
    if (foods.empty()) {
        printError("No products or recipes found.");
        return;
    }

    std::string recipeName;
    std::cout << "Recipe name: ";
    std::cin.ignore();
    std::getline(std::cin, recipeName);

    auto recipe = std::make_shared<Recipe>(recipeName);

    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        std::cout << "\n--- Available items (products and recipes) ---\n";
        for (size_t i = 0; i < foods.size(); ++i) {
            std::cout << i + 1 << ". " << foods[i]->getName();
            if (std::dynamic_pointer_cast<Recipe>(foods[i])) {
                std::cout << " (Recipe)";
            }
            std::cout << " - " << std::fixed << std::setprecision(1)
                      << foods[i]->getCalories() << " kcal/100g\n";
        }

        std::cout << "\nChoose item number (0 to finish): ";
        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        if (choice < 1 || choice > static_cast<int>(foods.size())) {
            printError("Invalid number!");
            continue;
        }

        auto selectedItem = foods[choice - 1];

        double grams;
        std::cout << "Weight (grams): ";
        std::cin >> grams;

        recipe->addIngredient(selectedItem, grams);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "Added: " << grams << "g " << selectedItem->getName();
        printSuccess(oss.str());

        std::cout << "\nAdd another? (y/n): ";
        std::cin >> addMore;
    }

    if (recipe->getIngredientCount() > 0) {
        tracker.addToFoodDatabase(recipe);
        tracker.saveData();
        printSuccess("Recipe \"" + recipeName + "\" created!");

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "Total ingredients: " << recipe->getIngredientCount() << "\n";
        oss << "Total weight: " << recipe->getTotalWeight() << "g\n";
        oss << "Calories: " << recipe->getCalories() << " kcal";
        std::cout << oss.str() << "\n";
    } else {
        printWarning("Recipe is empty, not created.");
    }
}

void showAllRecipes(CalorieTracker& tracker) {
    printTitle("ALL RECIPES");

    const auto& foods = tracker.getAllFoods();
    bool hasRecipes = false;

    for (const auto& food : foods) {
        if (auto recipe = std::dynamic_pointer_cast<Recipe>(food)) {
            std::cout << "\n" << BOLD << recipe->getName() << RESET << "\n";
            std::cout << "  Ingredients: " << recipe->getIngredientCount() << "\n";
            std::cout << "  Weight: " << recipe->getTotalWeight() << "g\n";
            std::cout << "  Calories: " << std::fixed << std::setprecision(1)
                      << recipe->getCalories() << " kcal\n";
            hasRecipes = true;
        }
    }

    if (!hasRecipes) {
        printWarning("No recipes found.");
    }
}

int main() {
    try {
        printWelcome();

        auto& tracker = CalorieTracker::getInstance();

        std::string username;
        std::cout << "Enter username (for data storage): ";
        std::getline(std::cin, username);

        tracker.initialize(username);

        if (!tracker.getUserProfile()) {
            std::cout << "\n👋 Welcome! Let's set up your profile.\n";
            setupProfile(tracker, username);
        }

        int choice;
        do {
            printMenu();
            std::cin >> choice;

            switch(choice) {
                case 1:
                    addNewProduct(tracker);
                    break;
                case 2:
                    showAllProducts(tracker);
                    break;
                case 3:
                    addNewMeal(tracker);
                    break;
                case 4:
                    showStatistics(tracker);
                    break;
                case 5:
                    createRecipe(tracker);
                    break;
                case 6:
                    showAllRecipes(tracker);
                    break;
                case 7:
                    if (auto user = tracker.getUserProfile()) {
                        setupProfile(tracker, user->getName());
                    } else {
                        setupProfile(tracker, username);
                    }
                    break;
                case 8:
                    showProfile(tracker);
                    break;
                case 0:
                    std::cout << "\nSaving data...\n";
                    tracker.saveData();
                    printSuccess("Goodbye!");
                    break;
                default:
                    printError("Invalid choice.");
            }

            if (choice != 0) {
                std::cout << "\nPress Enter to continue...";
                clearInput();
                std::cin.get();
            }

        } while (choice != 0);

    } catch (const std::exception& e) {
        std::cerr << RED << "\n❌ ERROR: " << e.what() << RESET << std::endl;
        return 1;
    }

    return 0;
}