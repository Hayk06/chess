//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Rook.h"
#include "../board/Board.h"

Rook::Rook(Color external): Piece(PieceType::ROOK, external, points[PieceType::ROOK]) {
    type = PieceType::ROOK;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}

std::vector<Position> Rook::moves(const Board& board, const Position from) const {
    return slide(board, from);
}

std::vector<Position> Rook::attacks(const Board& board, const Position from) const {
    return moves(board,from);
}
