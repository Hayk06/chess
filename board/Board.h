//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "Cell.h"

class Board {

    public:
        static constexpr int size = 8;

        Cell squares[size][size];

        Board();
        Board(const Board &board);
        void rotate(Board &board);
        void print() const;
        void display() const;

    Board& operator=(const Board &board) {
        if (this == &board) {
            return *this;
        }
        for ( int i = 0; i < board.size; ++i ) {
            for ( int j = 0; j < board.size; ++j ) {
                const Cell* cell = &board.squares[i][j];
                squares[i][j].name = cell->name;
                squares[i][j].piece = cell->piece ? cell->piece->clone() : nullptr;
                squares[i][j].color = cell->color;
            }
        }

        return *this;
    };
};
#endif //CHESS_BOARD_H
