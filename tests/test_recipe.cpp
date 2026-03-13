#include <gtest/gtest.h>
#include \"recipe.h\"
#include \"basic_food.h\"

TEST(RecipeTest, Constructor) {
    Recipe recipe(\"Test Recipe\");
    EXPECT_EQ(recipe.getName(), \"Test Recipe\");
    EXPECT_EQ(recipe.getIngredientCount(), 0);
    EXPECT_EQ(recipe.getTotalWeight(), 0.0);
}

TEST(RecipeTest, AddIngredient) {
    Recipe recipe(\"Test Recipe\");
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    
    recipe.addIngredient(apple, 200.0);
    
    EXPECT_EQ(recipe.getIngredientCount(), 1);
    EXPECT_DOUBLE_EQ(recipe.getTotalWeight(), 200.0);
}

TEST(RecipeTest, CaloriesCalculation) {
    Recipe recipe(\"Test Recipe\");
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    
    recipe.addIngredient(apple, 200.0);
    
    EXPECT_NEAR(recipe.getCalories(), 118.0, 0.1);
}

TEST(RecipeTest, ProteinCalculation) {
    Recipe recipe(\"Test Recipe\");
    auto chicken = std::make_shared<BasicFood>(\"Chicken\", 31.0, 3.6, 0.0, 1.0);
    
    recipe.addIngredient(chicken, 150.0);
    
    EXPECT_NEAR(recipe.getProtein(), 46.5, 0.1);
}

TEST(RecipeTest, FatCalculation) {
    Recipe recipe(\"Test Recipe\");
    auto chicken = std::make_shared<BasicFood>(\"Chicken\", 31.0, 3.6, 0.0, 1.0);
    
    recipe.addIngredient(chicken, 150.0);
    
    EXPECT_NEAR(recipe.getFat(), 5.4, 0.1);
}

TEST(RecipeTest, CarbsCalculation) {
    Recipe recipe(\"Test Recipe\");
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    
    recipe.addIngredient(apple, 200.0);
    
    EXPECT_NEAR(recipe.getCarbs(), 28.0, 0.1);
}

TEST(RecipeTest, UpdatePortion) {
    Recipe recipe(\"Test Recipe\");
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    
    recipe.addIngredient(apple, 200.0);
    recipe.updatePortion(2.0);
    
    EXPECT_DOUBLE_EQ(recipe.getTotalWeight(), 400.0);
    EXPECT_NEAR(recipe.getCalories(), 236.0, 0.1);
}

TEST(RecipeTest, Clone) {
    Recipe recipe(\"Test Recipe\");
    auto apple = std::make_shared<BasicFood>(\"Apple\", 0.3, 0.2, 14.0, 1.0);
    recipe.addIngredient(apple, 200.0);
    
    auto cloned = recipe.clone();
    
    EXPECT_EQ(cloned->getName(), recipe.getName());
    EXPECT_DOUBLE_EQ(cloned->getCalories(), recipe.getCalories());
}
