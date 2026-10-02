//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "../piece/Piece.h"

class Bishop : public Piece {
    public:
        Bishop(Color external);
        std::vector<Position> moves(const Board& board, Position from) const override;
        std::vector<Position> attacks(const Board& board, Position from) const override;
        std::unique_ptr<Piece> clone() const override;
};

#endif //CHESS_BISHOP_H
