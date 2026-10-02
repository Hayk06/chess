//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "King.h"
#include "../board/Board.h"

King::King(Color external) : Piece(PieceType::KING, external, points[PieceType::KING]) {
    type = PieceType::KING;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}

std::vector<Position> King::moves(const Board& board, const Position from) const {
    return slide(board, from);
}

std::vector<Position> King::attacks(const Board& board, const Position from) const {
    return moves(board,from);
}