//
// Created by Arsenii Fadieiev.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>

#define SIZE 9
#define CELL_SIZE 60
#define WINDOW_WIDTH (CELL_SIZE * SIZE)
#define WINDOW_HEIGHT (CELL_SIZE * SIZE + 60)

int numberOfSpaces = 0;
std::string diffName;
int selectedRow = -1;
int selectedCol = -1;
int originalBoard[SIZE][SIZE];
int currentBoard[SIZE][SIZE];
bool isPlayerEntered[SIZE][SIZE] = {false};
std::mutex boardMutex;
std::atomic<bool> gameWon(false);

//
void shuffleGame(int board[SIZE][SIZE]) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int group = 0; group < SIZE; group += 3) {
        for (int i = 0; i < 3; ++i) {
            int index1 = group + std::rand() % 3;
            int index2 = group + std::rand() % 3;
            if (index1 != index2) {
                for (int col = 0; col < SIZE; ++col) {
                    std::swap(board[index1][col], board[index2][col]);
                }
            }
        }
    }

    for (int group = 0; group < SIZE; group += 3) {
        for (int i = 0; i < 3; ++i) {
            int index1 = group + std::rand() % 3;
            int index2 = group + std::rand() % 3;
            if (index1 != index2) {
                for (int row = 0; row < SIZE; ++row) {
                    std::swap(board[row][index1], board[row][index2]);
                }
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            originalBoard[i][j] = board[i][j];
        }
    }
}

int chooseDifficulty() {
    std::string difficulty;
    std::cout << "Choose difficulty level (1-Easy, 2-Medium, 3-Hard): ";
    std::cin >> difficulty;

    if (difficulty == "1") {
        numberOfSpaces = 20;
        diffName = "Easy";
    } else if (difficulty == "2") {
        numberOfSpaces = 40;
        diffName = "Medium";
    } else if (difficulty == "3") {
        numberOfSpaces = 60;
        diffName = "Hard";
    } else{
        std::cout << "Error occurred. Default difficulty 'Idiot' was chosen." << std::endl;
        numberOfSpaces = 1;
        diffName = "Idiot";
    }

    return numberOfSpaces;
}

void displayGame(int board[SIZE][SIZE]) {
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            positions.emplace_back(i, j);
        }
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
    std::random_shuffle(positions.begin(), positions.end());

    for (int i = 0; i < numberOfSpaces; ++i) {
        int row = positions[i].first;
        int col = positions[i].second;
        board[row][col] = 0;
    }
}

bool isSudokuSolved(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] != originalBoard[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void drawBoard(sf::RenderWindow &window, int board[SIZE][SIZE], bool saveClicked, bool loadClicked) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    window.clear(sf::Color::White);

    for (int i = 0; i <= SIZE; ++i) {
        sf::RectangleShape line(sf::Vector2f(CELL_SIZE * SIZE, 2));
        line.setPosition(0, i * CELL_SIZE);
        line.setFillColor(sf::Color::Black);
        window.draw(line);

        line.setSize(sf::Vector2f(2, CELL_SIZE * SIZE));
        line.setPosition(i * CELL_SIZE, 0);
        window.draw(line);
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] > 0) {
                sf::Text text;
                text.setFont(font);
                text.setString(std::to_string(board[i][j]));
                text.setCharacterSize(24);
                if (isPlayerEntered[i][j]) {
                    text.setFillColor(sf::Color::Blue);
                } else {
                    text.setFillColor(sf::Color::Black);
                }

                text.setPosition(j * CELL_SIZE + 20, i * CELL_SIZE + 10);
                window.draw(text);
            }

            if (i == selectedRow && j == selectedCol) {
                sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                highlight.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow
                highlight.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                window.draw(highlight);
            }
        }
    }

    sf::Color saveColor = saveClicked ? sf::Color(150, 150, 150) : sf::Color(200, 200, 200);
    sf::Color loadColor = loadClicked ? sf::Color(150, 150, 150) : sf::Color(200, 200, 200);

    sf::RectangleShape saveButton(sf::Vector2f(100, 40));
    saveButton.setFillColor(sf::Color(200, 200, 200));
    saveButton.setPosition(WINDOW_WIDTH / 4 - 50, CELL_SIZE * SIZE + 20);

    sf::RectangleShape loadButton(sf::Vector2f(100, 40));
    loadButton.setFillColor(sf::Color(200, 200, 200));
    loadButton.setPosition(3 * (WINDOW_WIDTH / 4) - 50, CELL_SIZE * SIZE + 20);

    saveButton.setPosition(WINDOW_WIDTH / 4 - 50, CELL_SIZE * SIZE + 20);
    loadButton.setPosition(3 * (WINDOW_WIDTH / 4) - 50, CELL_SIZE * SIZE + 20);

    sf::Text saveText("Save", font, 20);
    saveText.setFillColor(sf::Color::Black);
    saveText.setPosition(saveButton.getPosition().x + 15, saveButton.getPosition().y + 5);

    sf::Text loadText("Load", font, 20);
    loadText.setFillColor(sf::Color::Black);
    loadText.setPosition(loadButton.getPosition().x + 15, loadButton.getPosition().y + 5);

    window.draw(saveButton);
    window.draw(loadButton);
    window.draw(saveText);
    window.draw(loadText);

    window.display();
}

void showCongratulations() {
    sf::RenderWindow congratsWindow(sf::VideoMode(400, 200), "Congratulations!");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font for congratulations window!" << std::endl;
        return;
    }

    sf::Text message;
    message.setFont(font);
    message.setString("Congratulations! >.<");
    message.setCharacterSize(24);
    message.setFillColor(sf::Color::Black);
    message.setPosition(20, 80);

    while (congratsWindow.isOpen()) {
        sf::Event event;
        while (congratsWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                congratsWindow.close();
            }
        }

        congratsWindow.clear(sf::Color::White);
        congratsWindow.draw(message);
        congratsWindow.display();
    }
}

void saveGame(const int board[SIZE][SIZE], const std::string &fileName) {
    std::thread saveThread([=]() {
        std::lock_guard<std::mutex> lock(boardMutex);
        std::ofstream outFile(fileName);
        if (!outFile) {
            std::cerr << "Failed to open file for saving!" << std::endl;
            return;
        }

        outFile << diffName << "\n";

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board[i][j] == 0) {
                    outFile << "0 ";
                } else if (isPlayerEntered[i][j]) {
                    outFile << -abs(board[i][j]) << " ";
                } else {
                    outFile << board[i][j] << " ";
                }
            }
            outFile << "\n";
        }

        outFile.close();
        std::cout << "Game saved to " << fileName << "!" << std::endl;
    });
    saveThread.join();
}

void loadGame(int board[SIZE][SIZE], const std::string &fileName) {
    std::thread loadThread([&, fileName]() {
        std::ifstream inFile(fileName);
        if (!inFile) {
            std::cerr << "Failed to open file for loading!" << std::endl;
            return;
        }

        std::string tempDiffName;
        int tempBoard[SIZE][SIZE];
        bool tempPlayerEntered[SIZE][SIZE];

        std::getline(inFile, tempDiffName);
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                int value;
                inFile >> value;
                if (value < 0) {
                    tempBoard[i][j] = -value;
                    tempPlayerEntered[i][j] = true;
                } else {
                    tempBoard[i][j] = value;
                    tempPlayerEntered[i][j] = false;
                }
            }
        }
        inFile.close();

        std::lock_guard<std::mutex> lock(boardMutex);
        diffName = tempDiffName;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                board[i][j] = tempBoard[i][j];
                isPlayerEntered[i][j] = tempPlayerEntered[i][j];
            }
        }

        std::cout << "Game loaded from " << fileName << "!" << std::endl;
    });

    loadThread.join();
}

void startWinCheckThread(int board[SIZE][SIZE]) {
    std::thread winCheckThread([&]() {
        while (!gameWon) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock(boardMutex);
            if (isSudokuSolved(board)) {
                gameWon = true;
                std::cout << "Congratulations, you won!" << std::endl;
            }
        }
    });
    winCheckThread.detach();
}


bool isButtonClicked(const sf::RectangleShape &button, const sf::Vector2i &mousePosition) {
    return button.getGlobalBounds().contains(sf::Vector2f(mousePosition));
}


int main() {
    int board[SIZE][SIZE] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    shuffleGame(board);
    chooseDifficulty();
    displayGame(board);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), diffName + " Level Sudoku");

    bool saveClicked = false;
    bool loadClicked = false;

    startWinCheckThread(board);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }


            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                sf::FloatRect saveArea(WINDOW_WIDTH / 4 - 50, CELL_SIZE * SIZE + 20, 100, 40);
                sf::FloatRect loadArea(3 * (WINDOW_WIDTH / 4) - 50, CELL_SIZE * SIZE + 20, 100, 40);

                if (saveArea.contains(static_cast<sf::Vector2f>(mousePos))) {
                    saveClicked = true;
                    saveGame(board, "save.txt");
                } else if (loadArea.contains(static_cast<sf::Vector2f>(mousePos))) {
                    loadClicked = true;
                    loadGame(board, "save.txt");
                }

                if (event.mouseButton.button == sf::Mouse::Left) {

                    saveClicked = false;
                    loadClicked = false;

                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;

                    if (board[y][x] <= 0 || isPlayerEntered[y][x]) {
                        selectedRow = y;
                        selectedCol = x;
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (selectedRow != -1 && selectedCol != -1) {
                    if (event.text.unicode >= '1' && event.text.unicode <= '9') {
                        int enteredValue = event.text.unicode - '0';

                        if (board[selectedRow][selectedCol] <= 0 || isPlayerEntered[selectedRow][selectedCol]) {
                            board[selectedRow][selectedCol] = enteredValue;
                            isPlayerEntered[selectedRow][selectedCol] = true;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace) {
                    if (selectedRow != -1 && selectedCol != -1) {
                        board[selectedRow][selectedCol] = 0;

                        if (isPlayerEntered[selectedRow][selectedCol]) {
                            board[selectedRow][selectedCol] = 0;
                            isPlayerEntered[selectedRow][selectedCol] = false;
                        }
                    }
                }
            }
        }

        if (isSudokuSolved(board)) {
            window.close();
            showCongratulations();
        }

        drawBoard(window, board, saveClicked, loadClicked);
    }

    return 0;
}