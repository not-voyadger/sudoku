Sudoku Game Implementation
Overview

This is a C++ implementation of a Sudoku game with a graphical interface using the SFML (Simple and Fast Multimedia Library). The game allows the user to play Sudoku, choose the difficulty level, shuffle the puzzle, and save/load the game state. The program includes multi-threading for saving and loading game data, as well as checking for a win condition.

Features:
Graphical User Interface (GUI): 
    Built using SFML to display the Sudoku board, numbers, and interactive buttons (Save and Load).
Difficulty Selection: 
    The player can choose from three difficulty levels: Easy, Medium, and Hard.
Shuffling: 
    The board is shuffled by swapping groups of rows and columns to create a new puzzle every time the game starts.
Game Save/Load: 
    The current game state can be saved to a simple text file and loaded later.
Win Condition: 
    The game checks whether the Sudoku puzzle is solved and displays a congratulatory message when the player wins.
Threading: 
    Separate threads are used for saving/loading game data and checking for a win condition.
