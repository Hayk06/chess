//
// Created by Hayk Derdzyan on 12.09.26.
//

#include "GameView.h"
#include "../board/BoardNameMap.h"
#include "../game/Game.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetricsF>
#include <QFont>
#include <QMouseEvent>
#include <algorithm>

namespace {

QString glyphFor(const Piece& piece)
{
    static const ushort solid[] = { 0, 0x265F, 0x265E, 0x265D, 0x265C, 0x265B, 0x265A };

    const int index = static_cast<int>(piece.type);
    if (index <= 0 || index > 6) {
        return {};
    }
    return QString(QChar(solid[index]));
}

} // namespace

GameView::GameView(QWidget* parent)
    : QWidget(parent)
{

}

void GameView::setGame(Game* newGame)
{
    game = newGame;
    flippedView = game && game->getSide().getColor() == Color::BLACK;
    update();
}

QSize GameView::sizeHint() const
{
    return { 640, 640 };
}

GameView::BoardGeometry GameView::boardGeometry() const
{
    const int squareSize  = qMin(width(), height()) / 8;
    const int boardPixels = squareSize * 8;

    return {
        squareSize,
        (width()  - boardPixels) / 2,
        (height() - boardPixels) / 2
    };
}

bool GameView::isFlipped() const
{
    return flippedView;
}

void GameView::toggleOrientation()
{
    flippedView = !flippedView;
    update();
}

bool GameView::squareAt(QPoint pos, int& row, int& col) const
{
    const BoardGeometry g = boardGeometry();
    if (g.squareSize <= 0) {
        return false;
    }

    const int x = pos.x() - g.originX;
    const int y = pos.y() - g.originY;
    if (x < 0 || y < 0) {
        return false;
    }

    const int screenCol = x / g.squareSize;
    const int screenRow = y / g.squareSize;
    if (screenRow > 7 || screenCol > 7) {
        return false;
    }

    row = isFlipped() ? 7 - screenRow : screenRow;
    col = isFlipped() ? 7 - screenCol : screenCol;
    return true;
}

void GameView::mousePressEvent(QMouseEvent* event)
{
    int row = 0;
    int col = 0;

    if (!squareAt(event->pos(), row, col)) {
        return;
    }

    if (game) {
        game->clickSquare({ row, col });
        update();
        emit boardChanged();
    }
}

void GameView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    const BoardGeometry g = boardGeometry();
    const int squareSize = g.squareSize;

    const QColor lightColor("#f0d9b5");
    const QColor darkColor("#b58863");

    const bool flipped = isFlipped();

    BoardNameMap letters;

    QFont labelFont = painter.font();
    labelFont.setPixelSize(qMax(8, squareSize / 5));
    labelFont.setBold(true);

    QFont pieceFont = painter.font();
    pieceFont.setPixelSize(qMax(12, squareSize * 7 / 8));

    const QFontMetricsF pieceMetrics(pieceFont);
    const int padding = qMax(2, squareSize / 16);

    const Color moverColor = game ? game->selectedColor() : Color::NONE;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            const int boardRow = flipped ? 7 - row : row;
            const int boardCol = flipped ? 7 - col : col;

            const bool isLight = (boardRow + boardCol) % 2 == 0;

            const QRect square(
                g.originX + col * squareSize,
                g.originY + row * squareSize,
                squareSize,
                squareSize
            );

            painter.fillRect(square, isLight ? lightColor : darkColor);

            if (game &&
                game->getCheckedMain().row == boardRow &&
                game->getCheckedMain().col == boardCol) {
                painter.fillRect(square, QColor(214, 48, 48, 110));
            }

            if (game &&
                game->getSelectedSquare().row == boardRow &&
                game->getSelectedSquare().col == boardCol) {
                painter.fillRect(square, QColor(246, 246, 105, 130));
            }

            painter.setFont(labelFont);
            painter.setPen(isLight ? darkColor : lightColor);

            if (col == 0) {
                painter.drawText(
                    square.adjusted(padding, padding, 0, 0),
                    Qt::AlignLeft | Qt::AlignTop,
                    QString::number(8 - boardRow)
                );
            }

            if (row == 7) {
                painter.drawText(
                    square.adjusted(0, 0, -padding, -padding),
                    Qt::AlignRight | Qt::AlignBottom,
                    QString(QChar(letters.map[boardCol]))
                );
            }

            if (!game) {
                continue;
            }

            const Piece* piece = game->getBoard().squares[boardRow][boardCol].piece.get();

            const std::vector<Position>& targets = game->getTargets();
            const bool isTarget = std::any_of(
                targets.begin(), targets.end(),
                [&](const Position& p) { return p.row == boardRow && p.col == boardCol; }
            );



            if (isTarget) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor(20, 90, 40, 110));

                if (piece) {
                    const bool friendly = piece->color == moverColor;

                    painter.setBrush(Qt::NoBrush);
                    painter.setPen(QPen(
                        friendly ? QColor(200, 45, 45, 190)
                                 : QColor(20, 130, 50, 190),
                        qMax(2, squareSize / 12)
                    ));
                }

                const int radius = piece ? squareSize / 2 - squareSize / 12
                                         : squareSize / 6;
                painter.drawEllipse(square.center(), radius, radius);
            }

            if (!piece) {
                continue;
            }

            const QString glyph = glyphFor(*piece);
            if (glyph.isEmpty()) {
                continue;
            }

            const QRectF bounds = pieceMetrics.boundingRect(glyph);
            const QPointF baseline(
                square.center().x() - bounds.width()  / 2.0 - bounds.left(),
                square.center().y() - bounds.height() / 2.0 - bounds.top()
            );

            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addText(baseline, pieceFont, glyph);

            const bool isBlackPiece = piece->color == Color::BLACK;

            painter.setPen(QPen(
                isBlackPiece ? QColor("#e8e8e8") : QColor("#1c1c1c"),
                qMax(1.0, squareSize / 64.0)
            ));
            painter.setBrush(isBlackPiece ? QColor("#1c1c1c") : QColor("#fdfdfa"));
            painter.drawPath(path);
        }
    }
}
