//
// Created by Hayk Derdzyan on 30.09.26.
//

#ifndef CHESS_PIECEFACTORY_H
#define CHESS_PIECEFACTORY_H

#include <memory>

#include "../piece/Piece.h"
#include "../shared/Color.h"

// Builds a concrete piece from a runtime type. Returns nullptr for
// PieceType::NONE or anything unrecognised.
std::unique_ptr<Piece> createPiece(PieceType type, Color color);

#endif //CHESS_PIECEFACTORY_H
