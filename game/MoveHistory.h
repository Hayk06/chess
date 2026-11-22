//
// Created by Hayk Derdzyan on 15.09.26.
//

#ifndef CHESS_MOVEHISTORY_H
#define CHESS_MOVEHISTORY_H

#include "../piece/PieceType.h"
#include "../shared/Color.h"
#include <string>

enum class CastleSide {
    NONE,
    KING,
    QUEEN
};

struct MoveHistory {
    std::string  from;
    std::string  to;
    PieceType movedType    = PieceType::NONE;
    Color     movedColor   = Color::NONE;
    PieceType capturedType = PieceType::NONE;
    Color     turn         = Color::NONE;
    bool      givesCheck   = false;
    PieceType promotedTo   = PieceType::NONE;
    CastleSide castle      = CastleSide::NONE;
    bool      enPassant    = false;
};
#endif //CHESS_MOVEHISTORY_H
