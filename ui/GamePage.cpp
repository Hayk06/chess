//
// Created by Hayk Derdzyan on 30.09.26.
//

#include "GamePage.h"
#include "PromotionDialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

GamePage::GamePage(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(12);

    topPanel    = new PlayerPanel;
    gameView    = new GameView;
    bottomPanel = new PlayerPanel;
    moveList    = new MoveList;

    auto* boardColumn = new QVBoxLayout;
    boardColumn->setSpacing(8);
    boardColumn->addWidget(topPanel);
    boardColumn->addWidget(gameView, 1);
    boardColumn->addWidget(bottomPanel);

    flipButton = new QPushButton("Flip board");

    auto* sideColumn = new QVBoxLayout;
    sideColumn->setSpacing(8);
    sideColumn->addWidget(moveList, 1);
    sideColumn->addWidget(flipButton);

    layout->addLayout(boardColumn, 1);
    layout->addLayout(sideColumn);

    connect(flipButton, &QPushButton::clicked,
            gameView, &GameView::toggleOrientation);

    connect(gameView, &GameView::boardChanged,
            this, &GamePage::onBoardChanged);
}

void GamePage::startGame(const Side side, const Timer timer)
{
    //todo refactor the init of main type
    game = std::make_unique<Game>(side, timer, PieceType::QUEEN);

    const int minutes = static_cast<int>(timer);
    const QString clock = QString("%1:00").arg(minutes, 2, 10, QChar('0'));

    const bool playingWhite = side.getColor() == Color::WHITE;

    topPanel->setPlayer(playingWhite ? "Player 2 (Black)" : "Player 2 (White)", clock);
    bottomPanel->setPlayer(playingWhite ? "Player 1 (White)" : "Player 1 (Black)", clock);

    gameView->setGame(game.get());
    moveList->clear();
}

void GamePage::onBoardChanged()
{
    if (game && game->getPendingPromotion().row >= 0) {
        QTimer::singleShot(0, this, &GamePage::handlePendingPromotion);
        return;
    }

    refreshHistory();
}

void GamePage::handlePendingPromotion()
{
    if (!game) {
        return;
    }

    const Position square = game->getPendingPromotion();
    if (square.row < 0) {
        return;
    }

    const Piece* pawn = game->getBoard().squares[square.row][square.col].piece.get();
    const Color color = pawn ? pawn->color : Color::WHITE;

    PromotionDialog dialog(color, this);
    dialog.exec();

    game->completePromotion(dialog.chosen());

    gameView->update();
    refreshHistory();
}

void GamePage::refreshHistory()
{
    if (game) {
        moveList->setHistory(game->getHistory());
    }
}
