//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Queen.h"
#include "../board/Board.h"

Queen::Queen(Color external) : Piece(PieceType::QUEEN, external, points[PieceType::QUEEN]) {
    type = PieceType::QUEEN;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

std::vector<Position> Queen::moves(const Board& board, const Position from) const {
    return slide(board, from);
}

std::vector<Position> Queen::attacks(const Board& board, const Position from) const {
    return moves(board,from);
}
