//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Pawn.h"
#include "../board/Board.h"

Pawn::Pawn(Color external) : Piece(PieceType::PAWN, external, points[PieceType::PAWN]) {
    type = PieceType::PAWN;
    color = external;
    point = points[type];
}

std::unique_ptr<Piece> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}

std::vector<Position> Pawn::moves(const Board& board, const Position from) const {
    std::vector<Position> result;

    const int step     = color == Color::WHITE ? -1 : 1;
    const int startRow = startingPosition.row;

    const int ahead = from.row + step;
    if (ahead < 0 || ahead >= boardSize) {
        return result;
    }

    if (!board.squares[ahead][from.col].piece) {
        result.push_back({ ahead, from.col });

        const int twoAhead = from.row + step * 2;
        if (from.row == startRow && !board.squares[twoAhead][from.col].piece) {
            result.push_back({ twoAhead, from.col });
        }
    }

    for (const int sideStep : { -1, 1 }) {
        const int col = from.col + sideStep;
        if (col < 0 || col >= boardSize) {
            continue;
        }

        const Piece* target = board.squares[ahead][col].piece.get();
        if (target && target->color != color) {
            result.push_back({ahead, col});
        }
    }

    return result;
}

std::vector<Position> Pawn::attacks(const Board& board, const Position from) const {
    std::vector<Position> result;

    const int ahead = from.row + (color == Color::WHITE ? -1 : 1);
    if (ahead < 0 || ahead >= boardSize) {
        return result;
    }

    for (const int sideStep : { -1, 1 }) {
        const int col = from.col + sideStep;
        if (col >= 0 && col < boardSize) {
            result.push_back({ ahead, col });
        }
    }
    return result;
}
