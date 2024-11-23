#include "Board.h"

#include <bit>
#include <format>

std::string moveToString(Move move) {
    int offset = std::countr_zero(move);
    int row = offset / 3;
    int col = offset % 3;
    return std::format("({}, {})", row, col);
}

std::string resultToString(Result result) {
    switch (result) {
        case Result::CROSS:
            return "CROSS";
        case Result::CIRCLE:
            return "CIRCLE";
        case Result::DRAW:
            return "DRAW";
        case Result::NONE:
            return "NONE";
    }
}

Board& Board::step(Move move) {
    boards[(int)turn] |= move;
    turn = (Player)(1 - (int)turn);
    moveStack.push(move);
    return *this;
}

Board& Board::undo() {
    Move move = moveStack.top();
    turn = (Player)(1 - (int)turn);
    boards[(int)turn] &= ~move;
    moveStack.pop();
    return *this;
}

Board& Board::reset() {
    while (!moveStack.empty()) {
        undo();
    }
    return *this;
}

Result Board::getWinner() {
    Result winner = Result::NONE;
    uint16_t fullMask = ~(boards[0] | boards[1]) & FULL_BOARD;
    if (!fullMask) {
        return Result::DRAW;
    }
    for (int i = 0; i < 2; i++) {
        for (const uint16_t& mask : WIN_MASKS) {
            uint16_t result = boards[i] & mask;
            if (std::popcount(result) == 3) {
                return (Result)i;
            }
        }
    }
    return winner;
}

std::vector<Move> Board::getMoves() {
    std::vector<Move> moves;
    if (getWinner() == Result::NONE) {
        uint16_t free = ~(boards[0] | boards[1]) & FULL_BOARD;
        while (free) {
            uint16_t mask = 1ULL << std::countr_zero(free);
            moves.push_back(mask);
            free &= ~mask;
        }
    }

    return moves;
}

std::string Board::toString() {
    std::string output = "+---+---+---+\n";
    uint16_t mask = 1;
    for (int i = 0; i < 3; i++) {
        for (int ii = 0; ii < 3; ii++) {
            std::string piece = "   ";
            if (boards[0] & mask) {
                piece = " X ";
            } else if (boards[1] & mask) {
                piece = " O ";
            }
            output += "|" + piece;
            mask <<= 1;
        }
        output += "|\n+---+---+---+\n";
    }
    return output;
}

Player Board::getTurn() {
    return turn;
}

double Board::getScore() {
    if (getWinner() == Result::CROSS) {
        return 1;
    } else if (getWinner() == Result::CIRCLE) {
        return -1;
    }
    return 0;
}