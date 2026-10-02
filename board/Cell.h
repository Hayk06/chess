//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_CELL_H
#define CHESS_CELL_H
#include <memory>
#include <string>
#include "../shared/Color.h"
#include "../piece/Piece.h"

class Cell {
    public:
        std::string name;
        std::unique_ptr<Piece> piece;
        Color color = Color::NONE;

        bool isEmpty() const {
            return piece == nullptr;
        }
};

#endif //CHESS_CELL_H
