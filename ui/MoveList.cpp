//
// Created by Hayk Derdzyan on 15.09.26.
//

#include "MoveList.h"
#include <QHeaderView>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

MoveList::MoveList(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    auto* title = new QLabel("Moves");
    QFont titleFont = title->font();
    titleFont.setBold(true);
    title->setFont(titleFont);

    table = new QTableWidget(0, 2);
    table->setHorizontalHeaderLabels({ "White", "Black" });
    table->verticalHeader()->setDefaultSectionSize(22);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(title);
    layout->addWidget(table, 1);
}

QSize MoveList::sizeHint() const
{
    return { 190, 400 };
}

void MoveList::clear()
{
    table->setRowCount(0);
}

QString MoveList::describe(const MoveHistory& move)
{
    static const char* letters = " NBRQK";

    QString text;
    if (move.movedType != PieceType::PAWN && move.movedType != PieceType::NONE) {
        text += QChar(letters[static_cast<int>(move.movedType) - 1]);
    }

    text += QString::fromStdString(move.from);
    text += move.capturedType == PieceType::NONE ? QChar('-') : QChar('x');
    text += QString::fromStdString(move.to);

    if (move.promotedTo != PieceType::NONE) {
        text += QChar('=');
        text += QChar(letters[static_cast<int>(move.promotedTo) - 1]);
    }

    if (move.givesCheck) {
        text += QChar('+');
    }

    return text;
}

void MoveList::setHistory(const std::vector<MoveHistory>& history)
{
    const int rows = static_cast<int>((history.size() + 1) / 2);
    table->setRowCount(rows);

    for (int i = 0; i < static_cast<int>(history.size()); ++i) {
        const int row = i / 2;
        const int col = i % 2;

        table->setItem(row, col, new QTableWidgetItem(describe(history[i])));
        table->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
    }

    if (rows > 0) {
        table->scrollToBottom();
    }
}
