#include <iostream>
template <typename T>
void input(T* t) {
    std::cin >> *t;
}

#feature on safety
#include "../vendor/std2.h"
using namespace std2;

void _pchar(char c) safe {
    unsafe { putchar(c); }
}

// Function to print the board
void printBoard(const vector<vector<char>>^ board) safe {
    println("-------------");
    for (unsigned int i = 0; i < 3; ++i) {
        const vector<char>^ row = board[i];
        print("| ");
    for (unsigned int j = 0; j < 3; ++j) {
        char cell = row[j];
            _pchar(cell);
            print(" | ");
        }
        println("");
        println("-------------");
    }
}

// Function to handle a player's move
void playerMove(vector<vector<char>>^ board, char player) safe {
    unsigned int row = 1, col = 1;
    while (true) {
        print("Player ");
        _pchar(player);
        print(", enter your move (row and column): ");
        unsafe {
            input(&row);
            input(&col);
        }
        if (row >= 1 && row <= 3 && col >= 1 && col <= 3 && board[row - 1][col - 1] == ' ') {
        //  board[row - 1][col - 1] = player;
            vector<char>^ column = ^board[row - 1];
            char^         cell   = ^column[col - 1];
                         *cell   = player;
            break;
        } else {
            println("Invalid move. Try again.");
        }
    }
}

// Function to check if a player has won
bool checkWin(const vector<vector<char>>^ board, char player) safe {
    // Check rows, columns, and diagonals
    for (unsigned int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) || // row
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))   // column
            return true;
    }
    return (board[0][0] == player && board[1][1] == player && board[2][2] == player) || // diagonal
           (board[0][2] == player && board[1][1] == player && board[2][0] == player);   // anti-diagonal
}

// Function to check if the game is a draw
bool checkDraw(const vector<vector<char>>^ board) safe {
    for (const vector<char>^ row : board) {
        for (char cell : row) {
            if (cell == ' ') return false;
        }
    }
    return true;
}

// Main function
int main() safe {
    // Initialize a 3x3 board with empty spaces
    vector<vector<char>> board {{
        vector<char>{{' ', ' ', ' '}},
        vector<char>{{' ', ' ', ' '}},
        vector<char>{{' ', ' ', ' '}}
    }};
    char currentPlayer = 'X';
    bool gameWon = false;

    println("Welcome to Tic Tac Toe!");

    while (true) {
        printBoard(board);

        // Take the player's move
        playerMove(^board, currentPlayer);

        // Check if the current player has won
        if (checkWin(board, currentPlayer)) {
            printBoard(board);
            print("Player ");
            print(currentPlayer);
            println(" wins!");
            break;
        }

        // Check if the game is a draw
        if (checkDraw(board)) {
            printBoard(board);
            println("It's a draw!");
            break;
        }

        // Switch to the other player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;
}
