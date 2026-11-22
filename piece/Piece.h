//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <memory>
#include <vector>

#include "PieceType.h"
#include "../shared/Color.h"
#include "../board/Position.h"

class Board;

class Piece {
    public:
        PieceType type = PieceType::NONE;
        Color color = Color::NONE;
        int point = 0;
        Position startingPosition;
        bool isMain = false;

        Piece(PieceType type, Color color, int point)
            : type(type), color(color), point(point) {}

        void setStartingPosition(Position position);
        void setMain(bool value);
        bool getMain() const;

        bool isEmpty() const { return type == PieceType::NONE; }

        char symbol();

        virtual std::vector<Position> moves(const Board& board, Position from) const = 0;
        virtual std::vector<Position> attacks(const Board& board, Position from) const = 0;
        virtual ~Piece() = default;

        virtual std::unique_ptr<Piece> clone() const = 0;
    protected:
        std::vector<Position> slide(const Board& board, Position from) const;

        Piece(const Piece&) = default;
        Piece& operator=(const Piece&) = default;
};

#endif //CHESS_PIECE_H
