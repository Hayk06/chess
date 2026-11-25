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
#include <array>
#include <vector>
#include <set>
#include <optional>

#include "MoveHistory.h"
#include <chrono>

enum class GameResult {
    Playing,
    Check,
    Checkmate,
    Stalemate,
    Timeout
};

struct CastleMove {
    Position kingTo;
    Position rookFrom;
    Position rookTo;
};

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

        const Side& getPlayer(Color color) const;

        Timer getTimer() const;
        PieceType getMainType() const;

        GameResult getResult() const;
        Color getWinner() const;
        bool isOver() const;
        bool isTimed() const;

        void tick(std::chrono::milliseconds elapsed);
        void setRemaining(Color color, std::chrono::milliseconds value);
        void setPlayerName(Color color, std::string name);

    private:
        Board board;
        Side side;
        std::array<Side, 3> players;
        std::map<Color, u_int64_t> attacks;
        PieceType main;
        Timer timer = Timer::FIVE;

        //todo add a piece member called main for king switch?

        Position selectedSquare;
        std::vector<Position> selectedTargets;
        std::vector<MoveHistory> history;
        Color currentTurn;
        Position checkedMain;
        Position pendingPromotion;
        Position enPassant;
        std::set<Position> castlingRights;
        bool timed = true;
        GameResult result = GameResult::Playing;
        Color winner = Color::NONE;

        Side& player(Color color);

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
        void initCastlingRights();
        std::vector<CastleMove> castleOptions(const Piece* piece, Position from);
        std::optional<CastleMove> castleFor(const Piece* piece, Position from, Position to);
        bool castlePathClear(Position from, Position rookFrom) const;
        bool castleKingPathSafe(Position from, Position kingTo);
        Position enPassantCapture(const Piece* piece, Position from, Position to) const;
        void markMainPieces();
        Position findMain(Color color) const;
        bool leavesMainAttacked(Position from, Position to, Position removed = {});
};

#endif //CHESS_GAME_H
