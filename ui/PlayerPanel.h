//
// Created by Hayk Derdzyan on 13.09.26.
//

#ifndef CHESS_PLAYERPANEL_H
#define CHESS_PLAYERPANEL_H
#include <QWidget>

class QLabel;

class PlayerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerPanel(QWidget* parent = nullptr);

    void setClock(const QString& clock);
    void setCaptured(const QString& pieces, int points);

private:
    QLabel* capturedLabel{};
    QLabel* clockLabel{};
};

#endif //CHESS_PLAYERPANEL_H
