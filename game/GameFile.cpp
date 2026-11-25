//
// Created by Hayk Derdzyan on 23.11.26.
//

#include "GameFile.h"

#include <fstream>

#include "Game.h"
#include "../board/Board.h"

namespace {

constexpr char pieceLetters[] = ".PNBRQK";

char letterFor(const PieceType type) {
    return pieceLetters[static_cast<int>(type)];
}

PieceType typeFor(const char letter) {
    for (int i = 0; i <= static_cast<int>(PieceType::KING); ++i) {
        if (pieceLetters[i] == letter) {
            return static_cast<PieceType>(i);
        }
    }
    return PieceType::NONE;
}

Position squareFor(const Board& board, const std::string& name) {
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (board.squares[row][col].name == name) {
                return { row, col };
            }
        }
    }
    return {};
}

}

bool GameFile::save(const Game& game, const std::string& path) {
    std::ofstream file(path);
    if (!file) {
        return false;
    }

    file << "chess 1\n";
    file << "side " << (game.getSide().getColor() == Color::BLACK ? "black" : "white") << "\n";
    file << "timer " << static_cast<int>(game.getTimer()) << "\n";
    file << "main " << letterFor(game.getMainType()) << "\n";
    file << "clock " << game.getPlayer(Color::WHITE).getRemaining().count()
         << " " << game.getPlayer(Color::BLACK).getRemaining().count() << "\n";

    for (const MoveHistory& record : game.getHistory()) {
        file << record.from << " " << record.to << " " << letterFor(record.promotedTo) << "\n";
    }

    return file.good();
}

std::unique_ptr<Game> GameFile::load(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return nullptr;
    }

    std::string key;
    int version = 0;

    if (!(file >> key >> version) || key != "chess" || version != 1) {
        return nullptr;
    }

    std::string colorName;
    if (!(file >> key >> colorName) || key != "side") {
        return nullptr;
    }

    int minutes = 0;
    if (!(file >> key >> minutes) || key != "timer") {
        return nullptr;
    }

    char mainLetter = '.';
    if (!(file >> key >> mainLetter) || key != "main") {
        return nullptr;
    }

    long long whiteClock = 0;
    long long blackClock = 0;
    if (!(file >> key >> whiteClock >> blackClock) || key != "clock") {
        return nullptr;
    }

    const PieceType mainType = typeFor(mainLetter);
    if (mainType == PieceType::NONE) {
        return nullptr;
    }

    Side side;
    side.setColor(colorName == "black" ? Color::BLACK : Color::WHITE);

    auto game = std::make_unique<Game>(side, static_cast<Timer>(minutes), mainType);

    std::string from;
    std::string to;
    char promotion = '.';

    while (file >> from >> to >> promotion) {
        const Position start = squareFor(game->getBoard(), from);
        const Position target = squareFor(game->getBoard(), to);

        if (start.row < 0 || target.row < 0) {
            return nullptr;
        }

        const size_t played = game->getHistory().size();

        game->clickSquare(start);
        game->clickSquare(target);

        if (game->getHistory().size() == played) {
            return nullptr;
        }

        if (game->getPendingPromotion().row >= 0) {
            game->completePromotion(typeFor(promotion));
        }
    }

    game->setRemaining(Color::WHITE, std::chrono::milliseconds(whiteClock));
    game->setRemaining(Color::BLACK, std::chrono::milliseconds(blackClock));

    return game;
}
