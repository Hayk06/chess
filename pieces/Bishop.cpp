//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Bishop.h"
#include "../board/Board.h"

Bishop::Bishop(Color external) : Piece(PieceType::BISHOP, external, points[PieceType::BISHOP]) {
    type = PieceType::BISHOP;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

std::vector<Position> Bishop::moves(const Board& board, const Position from) const {
    return slide(board, from);
}

std::vector<Position> Bishop::attacks(const Board& board, const Position from) const {
    return moves(board,from);
}
