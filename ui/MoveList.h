//
// Created by Hayk Derdzyan on 15.09.26.
//

#ifndef CHESS_MOVELIST_H
#define CHESS_MOVELIST_H
#include <QWidget>
#include <vector>

#include "../game/MoveHistory.h"

class QTableWidget;

class MoveList : public QWidget
{
    Q_OBJECT

public:
    explicit MoveList(QWidget* parent = nullptr);

    void setHistory(const std::vector<MoveHistory>& history);
    void clear();

    QSize sizeHint() const override;

private:
    static QString describe(const MoveHistory& move);

    QTableWidget* table{};
};

#endif //CHESS_MOVELIST_H
