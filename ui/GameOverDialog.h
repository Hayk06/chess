//
// Created by Hayk Derdzyan on 30.10.26.
//

#ifndef CHESS_GAMEOVERDIALOG_H
#define CHESS_GAMEOVERDIALOG_H
#include <QDialog>

class GameOverDialog : public QDialog
{
    Q_OBJECT

public:
    enum Action { Menu, Rematch };

    GameOverDialog(const QString& title, const QString& detail, QWidget* parent = nullptr);

    Action action() const { return chosen; }

private:
    Action chosen = Menu;
};

#endif //CHESS_GAMEOVERDIALOG_H
