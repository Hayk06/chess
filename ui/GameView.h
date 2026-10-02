//
// Created by Hayk Derdzyan on 12.09.26.
//

#ifndef CHESS_GAMEVIEW_H
#define CHESS_GAMEVIEW_H
#include <QWidget>
#include <vector>

#include "../board/Position.h"

class Game;

class GameView : public QWidget
{
    Q_OBJECT

public:
    explicit GameView(QWidget* parent = nullptr);

    void setGame(Game* game);

    void toggleOrientation();

    QSize sizeHint() const override;

signals:
    void boardChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    struct BoardGeometry {
        int squareSize;
        int originX;
        int originY;
    };

    BoardGeometry boardGeometry() const;
    bool isFlipped() const;
    bool squareAt(QPoint pos, int& row, int& col) const;

    Game* game{};
    bool flippedView = false;
};

#endif //CHESS_GAMEVIEW_H
