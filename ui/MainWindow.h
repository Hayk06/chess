//
// Created by Hayk Derdzyan on 12.09.26.
//

#ifndef CHESS_MAINWINDOW_H
#define CHESS_MAINWINDOW_H
#include <QMainWindow>

#include "GamePage.h"

class QComboBox;
class QRadioButton;
class QPushButton;
class QStackedWidget;
class QScreen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QComboBox* timerCombo{};
    QRadioButton* randomRadio{};
    QRadioButton* whiteRadio{};
    QRadioButton* blackRadio{};
    QPushButton* startButton{};
    QPushButton* loadButton{};

    QStackedWidget* pages;

    GamePage* gamePage{};

    void fitToCurrentPage(int index);
    void centerOnScreen();
    QScreen* currentScreen() const;
    void setupWelcomePage();
    void setupGamePage();
};
#endif //CHESS_MAINWINDOW_H
