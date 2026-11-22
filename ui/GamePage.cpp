//
// Created by Hayk Derdzyan on 30.09.26.
//

#include "GamePage.h"
#include "GameOverDialog.h"
#include "PromotionDialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <algorithm>

namespace {

QString formatClock(const std::chrono::milliseconds remaining)
{
    const long long seconds = remaining.count() / 1000;
    return QString("%1:%2")
        .arg(seconds / 60, 2, 10, QChar('0'))
        .arg(seconds % 60, 2, 10, QChar('0'));
}

QString capturedGlyphs(const std::vector<PieceType>& captured)
{
    static const ushort solid[] = { 0, 0x265F, 0x265E, 0x265D, 0x265C, 0x265B, 0x265A };

    std::vector<PieceType> sorted = captured;
    std::sort(sorted.begin(), sorted.end(), [](const PieceType a, const PieceType b) {
        return static_cast<int>(a) < static_cast<int>(b);
    });

    QString text;
    for (const PieceType type : sorted) {
        const int index = static_cast<int>(type);
        if (index > 0 && index <= 6) {
            text += QChar(solid[index]);
        }
    }
    return text;
}

}

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

    clockTimer = new QTimer(this);
    clockTimer->setInterval(100);
    connect(clockTimer, &QTimer::timeout, this, &GamePage::onClockTick);
}

void GamePage::startGame(const Side side, const Timer timer)
{
    game = std::make_unique<Game>(side, timer, PieceType::KING);

    const bool playingWhite = side.getColor() == Color::WHITE;

    bottomColor = playingWhite ? Color::WHITE : Color::BLACK;
    topColor    = playingWhite ? Color::BLACK : Color::WHITE;

    lastSide = side;
    lastTimer = timer;
    resultShown = false;

    gameView->setGame(game.get());
    moveList->clear();
    refreshPanels();

    if (game->isTimed()) {
        sinceTick.start();
        clockTimer->start();
    } else {
        clockTimer->stop();
    }
}

void GamePage::onClockTick()
{
    if (!game || game->isOver()) {
        clockTimer->stop();
        return;
    }

    if (game->getPendingPromotion().row >= 0) {
        sinceTick.restart();
        return;
    }

    game->tick(std::chrono::milliseconds(sinceTick.restart()));
    refreshPanels();

    if (game->isOver()) {
        clockTimer->stop();
        gameView->update();
        QTimer::singleShot(0, this, &GamePage::showResult);
    }
}

void GamePage::showResult()
{
    if (!game || resultShown) {
        return;
    }
    resultShown = true;

    const Color winner = game->getWinner();
    const QString side = winner == Color::WHITE ? "White" : "Black";

    QString title;
    QString detail;

    switch (game->getResult()) {
        case GameResult::Checkmate:
            title = "Checkmate";
            detail = QString("%1 wins").arg(side);
            break;
        case GameResult::Stalemate:
            title = "Stalemate";
            detail = "The game is a draw";
            break;
        case GameResult::Timeout:
            title = "Time out";
            detail = QString("%1 wins on time").arg(side);
            break;
        default:
            resultShown = false;
            return;
    }

    GameOverDialog dialog(title, detail, this);
    dialog.exec();

    if (dialog.action() == GameOverDialog::Rematch) {
        startGame(lastSide, lastTimer);
    } else {
        emit menuRequested();
    }
}

void GamePage::onBoardChanged()
{
    if (game && game->getPendingPromotion().row >= 0) {
        QTimer::singleShot(0, this, &GamePage::handlePendingPromotion);
        return;
    }

    refreshHistory();
    refreshPanels();

    if (game && game->isOver()) {
        clockTimer->stop();
        QTimer::singleShot(0, this, &GamePage::showResult);
    }
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
    refreshPanels();
}

void GamePage::refreshHistory()
{
    if (game) {
        moveList->setHistory(game->getHistory());
    }
}

void GamePage::refreshPanels()
{
    if (!game) {
        return;
    }

    const Side& top = game->getPlayer(topColor);
    const Side& bottom = game->getPlayer(bottomColor);

    const bool timed = game->isTimed();

    topPanel->setClock(timed ? formatClock(top.getRemaining()) : QString("--:--"));
    bottomPanel->setClock(timed ? formatClock(bottom.getRemaining()) : QString("--:--"));

    const int lead = top.get_captured_points() - bottom.get_captured_points();

    topPanel->setCaptured(capturedGlyphs(top.getCaptured()), lead > 0 ? lead : 0);
    bottomPanel->setCaptured(capturedGlyphs(bottom.getCaptured()), lead < 0 ? -lead : 0);
}
