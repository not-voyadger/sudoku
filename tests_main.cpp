#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

// Pseudocode for the chooseDifficulty function
extern int chooseDifficulty();
extern std::string diffName;
extern int numberOfSpaces;

void reset() {
    diffName = "";
    numberOfSpaces = 0;
}

TEST(ChooseDifficultyTest, EasyTest) {
    std::istringstream input("1");  // Simulating input "1"
    std::streambuf* original_cin = std::cin.rdbuf(input.rdbuf());  // Redirect std::cin

    reset();  // Reset values before the test
    int result = chooseDifficulty();  // Call the function being tested

    EXPECT_EQ(result, 20);  // Check that the number of spaces is set to 20
    EXPECT_EQ(diffName, "Easy");  // Check that the difficulty name is "Easy"

    std::cin.rdbuf(original_cin);  // Restore the original std::cin
}

TEST(ChooseDifficultyTest, MediumTest) {
    std::istringstream input("2");  // Simulate input "2"
    std::streambuf* original_cin = std::cin.rdbuf(input.rdbuf());

    reset();  // Reset values before the test
    int result = chooseDifficulty();

    EXPECT_EQ(result, 40);  // Check that the number of spaces is set to 40
    EXPECT_EQ(diffName, "Medium");  // Check that the difficulty name is "Medium"

    std::cin.rdbuf(original_cin);
}

TEST(ChooseDifficultyTest, HardTest) {
    std::istringstream input("3");  // Simulate input "3"
    std::streambuf* original_cin = std::cin.rdbuf(input.rdbuf());

    reset();  // Reset values before the test
    int result = chooseDifficulty();

    EXPECT_EQ(result, 60);  // Check that the number of spaces is set to 60
    EXPECT_EQ(diffName, "Hard");  // Check that the difficulty name is "Hard"

    std::cin.rdbuf(original_cin);
}


