//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Side.h"
#include "Timer.h"
#include "../board/Board.h"
#include "../board/Position.h"
#include <map>
#include <vector>
#include <set>

#include "MoveHistory.h"

class Game {
    public:
        Game(const Side side, const Timer timer , const PieceType main);
        void initBoard();

        void clickSquare(Position square);

        void completePromotion(PieceType type);

        Color selectedColor() const;


        //getters
        const Board& getBoard() const;
        const Side getSide() const;
        const Color getTurn() const ;
        const Position getSelectedSquare() const ;
        const std::vector<MoveHistory>& getHistory() const;
        const std::vector<Position>& getTargets() const ;

        Position getCheckedMain() const;

        Position getPendingPromotion() const;

    private:
        Board board;
        Timer timer;
        Side side;
        std::map<Color, u_int64_t> attacks;
        PieceType main;

        //todo add a piece member called main for king switch?

        Position selectedSquare;
        std::vector<Position> selectedTargets;
        std::vector<MoveHistory> history;
        Color currentTurn;
        Position checkedMain;
        Position pendingPromotion;

        void clearSelection();
        bool hasSelection() const;
        bool isTarget(Position square) const;
        void move(Position from, Position to);
        void setMoveToHistory(const MoveHistory& move);
        bool playerMoves();
        void end();
        std::set<Position> calcAttackFields(Color color);
        void initAttackFields();
        void checkForCheck();
        void printAttackBoard(uint64_t attacks);
        void changeTurn();
        std::vector<Position> filterMoves(const Piece* piece, Position from);
        bool checkPromotion(Piece* piece , Position to);
        bool attackPositionCheck(Position position);
        Color turnOpponent() const;
        Position findMain(Color color) const;
        bool leavesMainAttacked(Position from, Position to);
};

#endif //CHESS_GAME_H
