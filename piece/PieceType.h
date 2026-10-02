//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_PIECETYPE_H
#define CHESS_PIECETYPE_H

#include <ostream>
#include <map>
#include <vector>

#include "../board/Position.h"

enum class PieceType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};


inline std::ostream& operator<<(std::ostream& os, PieceType type) {
    static constexpr char symbols[] = ".PNBRQK";
    return os << symbols[static_cast<int>(type)];
}

inline std::map<PieceType, int> points = {
    {PieceType::PAWN, 1},
    {PieceType::KNIGHT, 3},
    {PieceType::BISHOP, 3},
    {PieceType::ROOK, 5},
    {PieceType::QUEEN, 9},
    {PieceType::KING, 0},
};

struct MovePattern {
    std::vector<Position> directions;
    int range = 1;
};

inline const std::map<PieceType, MovePattern> movePatterns = {
    { PieceType::KNIGHT, { { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
                             { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 } }, 1 } },

    { PieceType::BISHOP, { { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } },
                           boardSize - 1 } },

    { PieceType::ROOK,   { { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } },
                           boardSize - 1 } },

    { PieceType::QUEEN,  { { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
                             { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } },
                           boardSize - 1 } },

    { PieceType::KING,   { { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
                             { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } }, 1 } },
};

#endif //CHESS_PIECETYPE_H
