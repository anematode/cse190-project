#include "../vendor/io.h"
#include <array>
#include <string>
#include <vector>

#feature on safety

using namespace std;

// Define constants for board size
const unsigned int BOARD_SIZE = 8;

// Define a Board struct to hold the chessboard as a single array
struct Board {
    array<string, BOARD_SIZE * BOARD_SIZE> squares;

    Board() {
        for (unsigned int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            squares[i] = "";
        }
    }

    // Helper function to get the piece at a specific position
    string get(int x, int y) const {
        return cpy squares[(unsigned int)y * BOARD_SIZE + (unsigned int)x];
    }

    // Helper function to set a piece at a specific position
    void set(int x, int y, string piece) {
        println(x);
        println(y);
        println(piece);
        squares[(unsigned int)y * BOARD_SIZE + (unsigned int)x] = rel piece;
        println(x);
        println(y);
    }

    // Function to print the board
    void show() const {
        println("  a b c d e f g h");
        for (int i = 0; i < (int)BOARD_SIZE; ++i) {
            print(8 - i);
            print(" ");
            for (int j = 0; j < (int)BOARD_SIZE; ++j) {
                print(get(j, i) == "" ? "." : get(j, i));
                print(" ");
            }
            println(8 - i);
        }
        println("  a b c d e f g h");
    }
};

// Function to initialize the chess board
void initializeBoard(Board^ board) safe {
    Board& b = &(*board);
    
    // Place pawns
#if 1
    for (int i = 0; i < (int)BOARD_SIZE; ++i) {
        b.set(i, 1, "P");  // White pawns
        b.set(i, 6, "p");  // Black pawns
    }
#endif

    // Place other pieces
    vector<string> whitePieces = {"R", "N", "B", "Q", "K", "B", "N", "R"};
    vector<string> blackPieces = {"r", "n", "b", "q", "k", "b", "n", "r"};

#if 0
    for (int i = 0; i < (int)BOARD_SIZE; ++i) {
        b.set(i, 0, whitePieces[(unsigned int)i]);
        b.set(i, 7, blackPieces[(unsigned int)i]);
    }
#endif
}

// Abstract class for ValidMoveChecker
class ValidMoveChecker {
public:
    virtual bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const = 0;
    virtual ~ValidMoveChecker() {}
};

// Pawn checker class
class PawnChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        string piece = board.get(startX, startY);
        int direction = (piece == "P") ? 1 : -1;  // White moves up, black moves down

        // Pawn move: one square forward
        if (startX == endX && (startY + direction == endY) && board.get(endX, endY) == "") {
            return true;
        }

        // Pawn move: two squares forward from initial position
        if (startX == endX && (startY == (piece == "P" ? 1 : 6)) && (startY + 2 * direction == endY) && board.get(endX, endY) == "") {
            return true;
        }

        // Pawn capture
        if (abs(startX - endX) == 1 && startY + direction == endY && board.get(endX, endY) != "" && 
            ((piece == "P" && islower(board.get(endX, endY)[0])) || (piece == "p" && isupper(board.get(endX, endY)[0])))) {
            return true;
        }

        return false;
    }
};

// Rook checker class
class RookChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        if (startX == endX) {  // Vertical move
            for (int i = *min(startY, endY) + 1; i < *max(startY, endY); ++i) {
                if (board.get(startX, i) != "") return false;  // Blocked
            }
            return true;
        } else if (startY == endY) {  // Horizontal move
            for (int i = *min(startX, endX) + 1; i < *max(startX, endX); ++i) {
                if (board.get(i, startY) != "") return false;  // Blocked
            }
            return true;
        }
        return false;
    }
};

// Knight checker class
class KnightChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        if ((abs(startX - endX) == 2 && abs(startY - endY) == 1) || 
            (abs(startX - endX) == 1 && abs(startY - endY) == 2)) {
            return true;
        }
        return false;
    }
};

// Bishop checker class
class BishopChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        if (abs(startX - endX) == abs(startY - endY)) {  // Diagonal move
            int dx = (endX > startX) ? 1 : -1;
            int dy = (endY > startY) ? 1 : -1;
            int x = startX + dx, y = startY + dy;
            while (x != endX && y != endY) {
                if (board.get(x, y) != "") return false;  // Blocked
                x += dx;
                y += dy;
            }
            return true;
        }
        return false;
    }
};

// Queen checker class
class QueenChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        if (startX == endX || startY == endY) {  // Horizontal/Vertical move (rook's move)
            return RookChecker().isValidMove(startX, startY, endX, endY, board, player);
        } else if (abs(startX - endX) == abs(startY - endY)) {  // Diagonal move (bishop's move)
            return BishopChecker().isValidMove(startX, startY, endX, endY, board, player);
        }
        return false;
    }
};

// King checker class
class KingChecker : public ValidMoveChecker {
public:
    bool isValidMove(int startX, int startY, int endX, int endY, const Board& board, const string& player) const override {
        if (abs(startX - endX) <= 1 && abs(startY - endY) <= 1) {
            return true;
        }
        return false;
    }
};

// Factory to create the correct checker based on the piece type
class ValidMoveCheckerFactory {
public:
    static ValidMoveChecker* createChecker(char pieceType) {
        switch (tolower(pieceType)) {
            case 'p': return new PawnChecker();
            case 'r': return new RookChecker();
            case 'n': return new KnightChecker();
            case 'b': return new BishopChecker();
            case 'q': return new QueenChecker();
            case 'k': return new KingChecker();
            default:  return nullptr;
        }
    }
};

// Function to handle player move
void playerMove(Board& board, string player) {
    string move = {};
    while (true) {
        print(player);
        print("'s turn. Enter your move (e.g., e2 e4): ");
        getline(&cin, &move);

        // Ensure move has the correct length
        if (move.size() != 5 || move[2] != ' ') {
            println("Invalid format. Try again.");
            continue;
        }

        int startX = move[0] - 'a', startY = 8 - (move[1] - '0');
        int endX = move[3] - 'a', endY = 8 - (move[4] - '0');

        string piece = board.get(startX, startY);
        if (piece == "" || (isupper(piece[0]) && player == "Black") || (islower(piece[0]) && player == "White")) {
            println("Invalid move: not your piece.");
            continue;
        }

        ValidMoveChecker* checker = ValidMoveCheckerFactory::createChecker(piece[0]);
        if (checker && checker->isValidMove(startX, startY, endX, endY, board, player)) {
            board.set(endX, endY, rel piece);
            board.set(startX, startY, "");
            board.show();
            break;
        } else {
            println("Invalid move. Try again.");
        }
    }
}

int main() {
    Board board {};
    initializeBoard(^board);
    board.show();

    while (true) {
        playerMove(&board, "White");
        playerMove(&board, "Black");
    }

    return 0;
}
