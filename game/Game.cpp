//
// Created by Hayk Derdzyan on 10.09.26.
//

#include "Game.h"
#include <utility>

#include <iostream>
#include <algorithm>
#include <set>

#include "../pieces/PieceFactory.h"
#include "../pieces/Pawn.h"
#include "../pieces/Rook.h"
#include "../pieces/Bishop.h"
#include "../pieces/King.h"
#include "../pieces/Knight.h"
#include "../pieces/Queen.h"

Game::Game(const Side side, const Timer timer, const PieceType main) {
    const Board start;
    this->board = start;
    this->side = side;
    this->currentTurn = Color::WHITE;

    const auto budget = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::minutes(static_cast<int>(timer)));

    players[static_cast<int>(Color::WHITE)] = Side(Color::WHITE, "Player 1", budget);
    players[static_cast<int>(Color::BLACK)] = Side(Color::BLACK, "Player 2", budget);
    this->main = main;
    this->timer = timer;
    this->timed = timer != Timer::NONE;

    initBoard();
    initAttackFields();
    board.print();
}

void Game::initBoard() {
    for (int i = 0; i < Board::size; i++) {
        board.squares[1][i].piece = std::make_unique<Pawn>(Color::BLACK);
        board.squares[6][i].piece = std::make_unique<Pawn>(Color::WHITE);
    }
    board.squares[0][0].piece = std::make_unique<Rook>(Color::BLACK);
    board.squares[0][7].piece = std::make_unique<Rook>(Color::BLACK);

    board.squares[7][0].piece = std::make_unique<Rook>(Color::WHITE);
    board.squares[7][7].piece = std::make_unique<Rook>(Color::WHITE);

    board.squares[0][1].piece = std::make_unique<Knight>(Color::BLACK);
    board.squares[0][6].piece = std::make_unique<Knight>(Color::BLACK);

    board.squares[7][1].piece = std::make_unique<Knight>(Color::WHITE);
    board.squares[7][6].piece = std::make_unique<Knight>(Color::WHITE);

    board.squares[0][2].piece = std::make_unique<Bishop>(Color::BLACK);
    board.squares[0][5].piece = std::make_unique<Bishop>(Color::BLACK);

    board.squares[7][2].piece = std::make_unique<Bishop>(Color::WHITE);
    board.squares[7][5].piece = std::make_unique<Bishop>(Color::WHITE);

    board.squares[0][3].piece = std::make_unique<Queen>(Color::BLACK);
    board.squares[0][4].piece = std::make_unique<King>(Color::BLACK);

    board.squares[7][3].piece = std::make_unique<Queen>(Color::WHITE);
    board.squares[7][4].piece = std::make_unique<King>(Color::WHITE);

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board.squares[i][j].isEmpty()) continue;
            board.squares[i][j].piece->setStartingPosition({i,j});
        }
    }

    markMainPieces();
    initCastlingRights();
}

void Game::initCastlingRights() {
    castlingRights.clear();

    for (const Color color : { Color::WHITE, Color::BLACK }) {
        const Position mainSquare = findMain(color);
        if (mainSquare.row < 0) {
            continue;
        }

        castlingRights.insert(mainSquare);

        for (int col = 0; col < boardSize; ++col) {
            const Piece* piece = board.squares[mainSquare.row][col].piece.get();
            if (piece && piece->type == PieceType::ROOK && piece->color == color) {
                castlingRights.insert({ mainSquare.row, col });
            }
        }
    }
}

void Game::markMainPieces() {
    for (const Color color : { Color::WHITE, Color::BLACK }) {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                Piece* piece = board.squares[i][j].piece.get();
                if (piece && piece->type == main && piece->color == color) {
                    piece->setMain(true);
                    i = boardSize;
                    break;
                }
            }
        }
    }
}

std::set<Position> Game::calcAttackFields(Color color) {
    std::set<Position> temp = {};
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board.squares[i][j].piece == nullptr || board.squares[i][j].piece->color != color) continue;
            Piece* piece = board.squares[i][j].piece.get();
            std::vector<Position> pieceMoves = piece->attacks(board, {i,j});
            for (Position each : pieceMoves) {
                temp.insert(each);
            }
        }
    }
    return temp;
}

void Game::initAttackFields() {
    this->attacks[Color::WHITE] = 0;
    this->attacks[Color::BLACK] = 0;
    std::map<Color, std::set<Position>> fields{
            {Color::WHITE, {}},
            {Color::BLACK, {}}
    };

    for (const auto& [key, value] : fields) {
        std::set<Position> temp = calcAttackFields(key);
        fields[key] = temp;
        std::set<Position> positions = value;
        for(Position each : positions){
            int val = each.row * 8 + each.col;
            attacks[key] |= 1ULL << val;
        }
    }

    printAttackBoard(attacks[Color::WHITE]);
    printAttackBoard(attacks[Color::BLACK]);
}


void Game::clickSquare(const Position square) {
    if (isOver()) {
        return;
    }

    if (square.row < 0 || square.row >= Board::size ||
        square.col < 0 || square.col >= Board::size) {
        return;
    }

    if (pendingPromotion.row >= 0) {
        return;
    }

    const Piece* clicked = board.squares[square.row][square.col].piece.get();

    if (hasSelection() && isTarget(square)) {
        move(selectedSquare, square);
        clearSelection();
        return;
    }

    if (clicked && clicked->color != currentTurn) {
        return;
    }

    if (selectedSquare.row == square.row && selectedSquare.col == square.col) {
        clearSelection();
        return;
    }

    if (!clicked) {
        clearSelection();
        return;
    }

    selectedSquare = square;
    selectedTargets = filterMoves(clicked, square);
}

bool Game::hasSelection() const {
    return selectedSquare.row >= 0 && selectedSquare.col >= 0;
}

bool Game::isTarget(const Position square) const {
    for (const Position& target : selectedTargets) {
        if (target.row == square.row && target.col == square.col) {
            return true;
        }
    }
    return false;
}

void Game::clearSelection() {
    selectedSquare = {};
    selectedTargets.clear();
}

Color Game::selectedColor() const {
    if (selectedSquare.row < 0 || selectedSquare.col < 0) {
        return Color::NONE;
    }

    const Piece* piece = board.squares[selectedSquare.row][selectedSquare.col].piece.get();
    return piece ? piece->color : Color::NONE;
}

void Game::move(const Position from, const Position to) {
    std::unique_ptr<Piece>& source = board.squares[from.row][from.col].piece;
    if (!source) {
        return;
    }

    const bool promoting = checkPromotion(source.get(), to);
    const std::optional<CastleMove> castle = castleFor(source.get(), from, to);
    const Position passant = enPassantCapture(source.get(), from, to);

    const bool doubleStep = source->type == PieceType::PAWN &&
                            (to.row - from.row == 2 || to.row - from.row == -2);

    const Position capturedSquare = passant.row >= 0 ? passant : to;
    const Piece* captured = castle
        ? nullptr
        : board.squares[capturedSquare.row][capturedSquare.col].piece.get();
    std::string fromName = board.squares[from.row][from.col].name;
    std::string toName = board.squares[to.row][to.col].name;
    MoveHistory record{
        fromName, toName,
        source->type,
        source->color,
        captured ? captured->type : PieceType::NONE,
        currentTurn
    };

    if (castle) {
        record.castle = castle->rookFrom.col < from.col ? CastleSide::QUEEN : CastleSide::KING;
    }

    record.enPassant = passant.row >= 0;

    setMoveToHistory(record);

    if (captured) {
        player(currentTurn).addCapture(captured->type);
    }

    if (castle) {
        std::unique_ptr<Piece> rook =
            std::move(board.squares[castle->rookFrom.row][castle->rookFrom.col].piece);
        board.squares[to.row][to.col].piece = std::move(source);
        board.squares[castle->rookTo.row][castle->rookTo.col].piece = std::move(rook);
        castlingRights.erase(castle->rookFrom);
    } else {
        board.squares[to.row][to.col].piece = std::move(source);
    }

    if (passant.row >= 0) {
        board.squares[passant.row][passant.col].piece.reset();
    }

    enPassant = doubleStep ? Position{ (from.row + to.row) / 2, from.col } : Position{};

    castlingRights.erase(from);
    castlingRights.erase(to);

    if (promoting) {
        pendingPromotion = to;
        return;
    }

    changeTurn();
}

void Game::completePromotion(const PieceType type) {
    if (pendingPromotion.row < 0 || pendingPromotion.col < 0) {
        return;
    }

    std::unique_ptr<Piece>& square = board.squares[pendingPromotion.row][pendingPromotion.col].piece;

    if (square) {
        const Color color = square->color;

        if (std::unique_ptr<Piece> promoted = createPiece(type, color)) {
            square = std::move(promoted);

            if (!history.empty()) {
                history.back().promotedTo = type;
            }
        }
    }

    pendingPromotion = {};
    changeTurn();
}

bool Game::checkPromotion(Piece* piece , Position to) {
    bool type = piece->type == PieceType::PAWN;
    int row = piece->color == Color::WHITE ? 0 : 7;
    return to.row == row && type;
}

void Game::changeTurn() {
    currentTurn = currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
    initAttackFields();
    checkForCheck();

    const bool inCheck = checkedMain.row >= 0;

    if (!playerMoves()) {
        result = inCheck ? GameResult::Checkmate : GameResult::Stalemate;
        winner = inCheck ? turnOpponent() : Color::NONE;
        end();
        return;
    }

    result = inCheck ? GameResult::Check : GameResult::Playing;
//    board.rotate(board);
}

void Game::checkForCheck() {
    Color turn = getTurn();
    Position position = findMain(turn);

    checkedMain = {};

    if (position.row >= 0 && attackPositionCheck(position)) {
        checkedMain = position;

        if (!history.empty()) {
            history.back().givesCheck = true;
        }
    }
}

bool Game::playerMoves() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            Piece* piece = board.squares[i][j].piece.get();
            if (piece == nullptr) continue;
            if (piece->color != getTurn()) continue;
            std::vector moves = filterMoves(piece, {i , j});
            if (!moves.empty()) return true;
        }
    }
    return false;
}

void Game::setMoveToHistory(const MoveHistory& move) {
    history.push_back(move);
}

void Game::end() {
    std::cout<<"game ended"<<std::endl;

}

void Game::printAttackBoard(uint64_t attacks1)
{
    std::cout<<std::endl;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int square = row * 8 + col;

            if (attacks1 & (1ULL << square))
                std::cout << "1 ";
            else
                std::cout << "0 ";
        }

        std::cout << '\n';
    }
}

bool Game::attackPositionCheck(Position position) {
    const int square = position.row * 8 + position.col;
    return (attacks[turnOpponent()] & (1ULL << square)) != 0;
}


std::vector<Position> Game::filterMoves(const Piece* piece, const Position from) {
    std::vector<Position> moves = piece->moves(board, from);

    for (const CastleMove& option : castleOptions(piece, from)) {
        moves.push_back(option.kingTo);
    }

    if (enPassantCapture(piece, from, enPassant).row >= 0) {
        moves.push_back(enPassant);
    }

    std::erase_if(moves, [this, piece, from](const Position& to) {
        return leavesMainAttacked(from, to, enPassantCapture(piece, from, to));
    });

    return moves;
}

std::vector<CastleMove> Game::castleOptions(const Piece* piece, const Position from) {
    std::vector<CastleMove> options;

    if (!piece || !piece->getMain() || !castlingRights.contains(from)) {
        return options;
    }

    if (attackPositionCheck(from)) {
        return options;
    }

    for (int col = 0; col < boardSize; ++col) {
        const Position rookFrom{ from.row, col };

        if (col == from.col || !castlingRights.contains(rookFrom)) {
            continue;
        }

        const Piece* rook = board.squares[rookFrom.row][rookFrom.col].piece.get();
        if (!rook || rook->type != PieceType::ROOK || rook->color != piece->color) {
            continue;
        }

        const int step = col < from.col ? -1 : 1;
        const Position kingTo{ from.row, from.col + 2 * step };
        const Position rookTo{ from.row, from.col + step };

        if (kingTo.col < 0 || kingTo.col >= boardSize) {
            continue;
        }

        const Piece* landing = board.squares[kingTo.row][kingTo.col].piece.get();
        if (landing && landing != rook) {
            continue;
        }

        if (!castlePathClear(from, rookFrom) || !castleKingPathSafe(from, kingTo)) {
            continue;
        }

        options.push_back({ kingTo, rookFrom, rookTo });
    }

    return options;
}

Position Game::enPassantCapture(const Piece* piece, const Position from, const Position to) const {
    if (!piece || piece->type != PieceType::PAWN || enPassant.row < 0 || to != enPassant) {
        return {};
    }

    if (!board.squares[to.row][to.col].isEmpty()) {
        return {};
    }

    const int step = piece->color == Color::WHITE ? -1 : 1;
    const int sideStep = to.col - from.col;

    if (to.row != from.row + step || (sideStep != 1 && sideStep != -1)) {
        return {};
    }

    const Position victimSquare{ from.row, to.col };
    const Piece* victim = board.squares[victimSquare.row][victimSquare.col].piece.get();

    if (!victim || victim->type != PieceType::PAWN || victim->color == piece->color) {
        return {};
    }

    return victimSquare;
}

std::optional<CastleMove> Game::castleFor(const Piece* piece, const Position from, const Position to) {
    for (const CastleMove& option : castleOptions(piece, from)) {
        if (option.kingTo == to) {
            return option;
        }
    }
    return std::nullopt;
}

bool Game::castlePathClear(const Position from, const Position rookFrom) const {
    const int step = rookFrom.col > from.col ? 1 : -1;

    for (int col = from.col + step; col != rookFrom.col; col += step) {
        if (!board.squares[from.row][col].isEmpty()) {
            return false;
        }
    }
    return true;
}

bool Game::castleKingPathSafe(const Position from, const Position kingTo) {
    const int step = kingTo.col > from.col ? 1 : -1;

    for (int col = from.col + step; col != kingTo.col; col += step) {
        if (attackPositionCheck({ from.row, col })) {
            return false;
        }
    }
    return true;
}

Position Game::findMain(const Color color) const {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            const Piece* piece = board.squares[i][j].piece.get();
            if (piece && piece->getMain() && piece->color == color) {
                return { i, j };
            }
        }
    }
    return {};
}

bool Game::leavesMainAttacked(const Position from, const Position to, const Position removed) {
    std::unique_ptr<Piece>& source = board.squares[from.row][from.col].piece;
    std::unique_ptr<Piece>& target = board.squares[to.row][to.col].piece;

    if (!source) {
        return false;
    }

    const Color mover = source->color;

    std::unique_ptr<Piece> moving   = std::move(source);
    std::unique_ptr<Piece> captured = std::move(target);
    target = std::move(moving);

    std::unique_ptr<Piece> passed;
    if (removed.row >= 0) {
        passed = std::move(board.squares[removed.row][removed.col].piece);
    }

    bool attacked = false;
    const Position main = findMain(mover);
    if (main.row >= 0) {
        const std::set<Position> enemyAttacks =
            calcAttackFields(mover == Color::WHITE ? Color::BLACK : Color::WHITE);
        attacked = enemyAttacks.count(main) > 0;
    }

    if (removed.row >= 0) {
        board.squares[removed.row][removed.col].piece = std::move(passed);
    }

    source = std::move(target);
    target = std::move(captured);

    return attacked;
}

Color Game::turnOpponent() const {
    Color color = Color::WHITE;
    if (currentTurn == color) {
        color = Color::BLACK;
    }
    return color;
}

const Board& Game::getBoard() const { return board; }
const Side Game::getSide() const { return side; }
const Color Game::getTurn() const { return currentTurn; }
const Position Game::getSelectedSquare() const { return selectedSquare; }
const std::vector<MoveHistory>& Game::getHistory() const { return history; }
const std::vector<Position>& Game::getTargets() const { return selectedTargets; }
Position Game::getCheckedMain() const { return checkedMain; }
Position Game::getPendingPromotion() const { return pendingPromotion; }

Side& Game::player(const Color color) {
    return players[static_cast<int>(color)];
}

Timer Game::getTimer() const { return timer; }
PieceType Game::getMainType() const { return main; }

GameResult Game::getResult() const { return result; }
Color Game::getWinner() const { return winner; }

bool Game::isTimed() const { return timed; }

bool Game::isOver() const {
    return result == GameResult::Checkmate
        || result == GameResult::Stalemate
        || result == GameResult::Timeout;
}

void Game::tick(const std::chrono::milliseconds elapsed) {
    if (!timed || isOver()) {
        return;
    }

    Side& active = player(currentTurn);
    const std::chrono::milliseconds left = active.getRemaining() - elapsed;

    if (left <= std::chrono::milliseconds::zero()) {
        active.setRemaining(std::chrono::milliseconds::zero());
        result = GameResult::Timeout;
        winner = turnOpponent();
        end();
        return;
    }

    active.setRemaining(left);
}

const Side& Game::getPlayer(const Color color) const {
    return players[static_cast<int>(color)];
}

void Game::setRemaining(const Color color, const std::chrono::milliseconds value) {
    player(color).setRemaining(value);
}

void Game::setPlayerName(const Color color, std::string name) {
    player(color).setName(std::move(name));
}

//todo error and fix , in ui display check ++
//the pieces moves function logic should be change and each piece should return the vector of all positions that it
//physically can have , the game should define rules for some pieces , and the validations should be there
//the move logic is the slide function , by default piece->moves calls the slide from inside , mission is to extract that slide from there
//++
//add an interation for after move to update the attack lines +++

//change init attack logic ++