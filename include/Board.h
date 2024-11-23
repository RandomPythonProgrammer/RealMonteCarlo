#pragma once

#include <array>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>

enum class Player {
    CROSS,
    CIRCLE,
    NONE,
};

enum class Result {
    CROSS,
    CIRCLE,
    DRAW,
    NONE,
};

typedef uint16_t Move;

std::string moveToString(Move move);
std::string resultToString(Result result);

const std::array<uint16_t, 8> WIN_MASKS = {
    0b0000000111000000,
    0b0000000000111000,
    0b0000000000000111,
    0b0000000100100100,
    0b0000000010010010,
    0b0000000001001001,
    0b0000000100010001,
    0b0000000001010100
};

const uint16_t FULL_BOARD = 0b0000000111111111;

class Board {
   private:
    std::array<uint16_t, 2> boards = {0, 0};
    Player turn = Player::CROSS;
    std::stack<Move> moveStack;

   public:
    Board& step(Move move);
    Board& undo();
    Board& reset();
    Result getWinner();
    std::vector<Move> getMoves();
    std::string toString();
    Player getTurn();
    double getScore();
};