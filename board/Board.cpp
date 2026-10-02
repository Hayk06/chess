//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Board.h"
#include <string>

#include "BoardNameMap.h"
#include "../pieces/Pawn.h"
#include <iostream>

Board::Board() : squares{} {
    BoardNameMap letters;
    for ( int i = 0; i < size; ++i ) {
        for ( int j = 0; j < size; ++j ) {
            Cell* cell = &squares[i][j];
            cell->name = std::string{
                letters.map[j],
                static_cast<char>('0' + size-i),
            };
            cell->color = (i + j) % 2 == 0 ? Color::WHITE : Color::BLACK;
        }
    }
}

Board::Board(const Board& board) {
    for ( int i = 0; i < size; ++i ) {
        for ( int j = 0; j < size; ++j ) {
            const Cell& cell = board.squares[i][j];
            squares[i][j].name = cell.name;
            squares[i][j].piece = cell.piece ? cell.piece->clone() : nullptr;
            squares[i][j].color = cell.color;
        }
    }

}

void Board::rotate(Board &board) {
    Board temp(board);
    for ( int i = 0; i < size; ++i ) {
        for (int j = 0; j < size; ++j ) {
            const Cell& cell = temp.squares[i][j];
            squares[board.size - i - 1][board.size -j -1].name = cell.name;
        }
    }

}

void Board::print() const {

    for ( int i = 0; i < size; ++i ) {
        for ( int j = 0; j < size; ++j ) {
            const Cell& cell = squares[i][j];
            std::cout << cell.name
                      << (cell.color == Color::BLACK ? 'B' : 'W') << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}

void Board::display() const {

    for ( int i = 0; i < size; ++i ) {
        std::cout << size - i << "  ";
        for ( int j = 0; j < size; ++j ) {
            const Cell& cell = squares[i][j];
            std::cout << (cell.piece ? cell.piece->symbol() : '.') << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\n   ";
    for ( int j = 0; j < size; ++j ) {
        std::cout << static_cast<char>('a' + j) << ' ';
    }
    std::cout << std::endl;
}
