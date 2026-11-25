//
// Created by Hayk Derdzyan on 23.11.26.
//

#ifndef CHESS_GAMEFILE_H
#define CHESS_GAMEFILE_H

#include <memory>
#include <string>

class Game;

namespace GameFile {
    bool save(const Game& game, const std::string& path);
    std::unique_ptr<Game> load(const std::string& path);
}

#endif //CHESS_GAMEFILE_H
