//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

inline constexpr int boardSize = 8;

struct Position {
    int row = -1;
    int col = -1;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator<(const Position& other) const {
        if (row != other.row)
            return row < other.row;

        return col < other.col;
    }
};

#endif //CHESS_POSITION_H
