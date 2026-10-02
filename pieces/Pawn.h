//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "../piece/Piece.h"

class Pawn : public Piece {
    public:
        Pawn(Color external);
        std::vector<Position> moves(const Board& board, Position from) const override;
        std::vector<Position> attacks(const Board& board, Position from) const override;
        std::unique_ptr<Piece> clone() const override;
};

#endif //CHESS_PAWN_H
