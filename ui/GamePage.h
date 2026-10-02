//
// Created by Hayk Derdzyan on 30.09.26.
//

#ifndef CHESS_GAMEPAGE_H
#define CHESS_GAMEPAGE_H
#include <QWidget>
#include <memory>

#include "GameView.h"
#include "MoveList.h"
#include "PlayerPanel.h"
#include "../game/Game.h"
#include "../game/Side.h"
#include "../game/Timer.h"

class QPushButton;

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget* parent = nullptr);

    void startGame(Side side, Timer timer);

private:
    void onBoardChanged();
    void handlePendingPromotion();
    void refreshHistory();

    GameView* gameView{};
    PlayerPanel* topPanel{};
    PlayerPanel* bottomPanel{};
    MoveList* moveList{};
    QPushButton* flipButton{};

    std::unique_ptr<Game> game{};
};

#endif //CHESS_GAMEPAGE_H
