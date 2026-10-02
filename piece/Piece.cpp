//
// Created by Hayk Derdzyan on 15.09.26.
//

#include "Piece.h"
#include "../board/Board.h"

std::vector<Position> Piece::slide(const Board& board, const Position from) const
{
    const auto found = movePatterns.find(type);
    if (found == movePatterns.end()) {
        return {};
    }

    const MovePattern& pattern = found->second;
    std::vector<Position> result;

    for (const Position& direction : pattern.directions) {
        for (int step = 1; step <= pattern.range; ++step) {
            const int row = from.row + direction.row * step;
            const int col = from.col + direction.col * step;

            if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
                break;
            }

            const Piece* occupant = board.squares[row][col].piece.get();

            if (occupant) {
                if (occupant->color != color) {
                    result.push_back({ row, col });
                }
                break;
            }

            result.push_back({ row, col });
        }
    }

    return result;
}

void Piece::setStartingPosition(Position position) {
        startingPosition = position;
}

char Piece::symbol() {
    static constexpr char symbols[] = ".PNBRQK";
    if (type == PieceType::NONE) return '.';
    const char s = symbols[static_cast<int>(type)];
    return color == Color::BLACK ? static_cast<char>(s - 'A' + 'a') : s;
}