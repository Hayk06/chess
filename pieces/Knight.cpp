//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Knight.h"
#include "../board/Board.h"

Knight::Knight(Color external) : Piece(PieceType::KNIGHT, external, points[PieceType::KNIGHT]) {
    type = PieceType::KNIGHT;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

std::vector<Position> Knight::moves(const Board& board, const Position from) const {
    return slide(board, from);
}

std::vector<Position> Knight::attacks(const Board& board, const Position from) const {
    return moves(board,from);
}

//todo directrions move to map to make dynamic +
//todo integrate db ?
