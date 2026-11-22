//
// Created by Hayk Derdzyan on 12.09.26.
//
#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include "../game/Side.h"
#include "../game/Timer.h"
#include "../game/Game.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    pages = new QStackedWidget(this);
    setCentralWidget(pages);

    setupWelcomePage();
    setupGamePage();
    connect(pages, &QStackedWidget::currentChanged,
            this, &MainWindow::fitToCurrentPage);

    pages->setCurrentIndex(0);
    fitToCurrentPage(0);
}

void MainWindow::fitToCurrentPage(int index)
{
    for (int i = 0; i < pages->count(); ++i) {
        const auto policy = (i == index)
                                ? QSizePolicy::Preferred
                                : QSizePolicy::Ignored;
        pages->widget(i)->setSizePolicy(policy, policy);
    }

    QWidget* page = pages->widget(index);
    page->adjustSize();

    setMinimumSize(page == gamePage ? QSize(380, 560) : QSize(0, 0));

    QSize target = page->sizeHint();

    if (const QScreen* screen = currentScreen()) {
        const QRect available = screen->availableGeometry();
        target.setWidth (qMin(target.width(),  available.width()  - 60));
        target.setHeight(qMin(target.height(), available.height() - 60));
    }

    resize(target);
    centerOnScreen();
}

QScreen* MainWindow::currentScreen() const
{
    if (windowHandle() && windowHandle()->screen()) {
        return windowHandle()->screen();
    }
    return QGuiApplication::primaryScreen();
}

void MainWindow::centerOnScreen()
{
    const QScreen* screen = currentScreen();
    if (!screen) {
        return;
    }

    const QRect available = screen->availableGeometry();

    QRect frame = frameGeometry();
    if (frame.isEmpty()) {
        frame = QRect(pos(), size());
    }
    frame.moveCenter(available.center());

    move(frame.topLeft());
}

void MainWindow::setupWelcomePage()
{
    auto* page = new QWidget;

    auto* layout = new QVBoxLayout(page);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(16);

    auto* title = new QLabel("Welcome to Chess");
    title->setAlignment(Qt::AlignCenter);

    auto* sideLabel = new QLabel("Choose your side:");

    whiteRadio = new QRadioButton("White");
    blackRadio = new QRadioButton("Black");
    randomRadio = new QRadioButton("Random");

    whiteRadio->setChecked(true);

    auto* sideGroup = new QButtonGroup(this);
    sideGroup->addButton(whiteRadio);
    sideGroup->addButton(blackRadio);
    sideGroup->addButton(randomRadio);

    auto* sideLayout = new QHBoxLayout;
    sideLayout->setSpacing(12);
    sideLayout->addWidget(whiteRadio);
    sideLayout->addWidget(blackRadio);
    sideLayout->addWidget(randomRadio);

    auto* timerLabel = new QLabel("Choose your timer:");

    timerCombo = new QComboBox;
    timerCombo->addItem("1 minute", static_cast<int>(Timer::ONE));
    timerCombo->addItem("3 minutes", static_cast<int>(Timer::THREE));
    timerCombo->addItem("5 minutes", static_cast<int>(Timer::FIVE));
    timerCombo->addItem("10 minutes", static_cast<int>(Timer::TEN));
    timerCombo->addItem("No limit", static_cast<int>(Timer::NONE));

    startButton = new QPushButton("Start Game");

    layout->addWidget(title);
    layout->addWidget(sideLabel);
    layout->addLayout(sideLayout);
    layout->addWidget(timerLabel);
    layout->addWidget(timerCombo);
    layout->addWidget(startButton);

    pages->addWidget(page);

    connect(startButton, &QPushButton::clicked, this, [this]()
    {
        Side side;

        if (whiteRadio->isChecked())
            side.setColor(Color::WHITE);
        else if (blackRadio->isChecked())
            side.setColor(Color::BLACK);
        else
            side.randomSide();

        Timer timer =
            static_cast<Timer>(timerCombo->currentData().toInt());

        gamePage->startGame(side, timer);

        pages->setCurrentWidget(gamePage);
    });
}

void MainWindow::setupGamePage()
{
    gamePage = new GamePage;
    pages->addWidget(gamePage);

    connect(gamePage, &GamePage::menuRequested, this, [this]() {
        pages->setCurrentIndex(0);
    });
}

