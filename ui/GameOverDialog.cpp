//
// Created by Hayk Derdzyan on 30.10.26.
//

#include "GameOverDialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

GameOverDialog::GameOverDialog(const QString& title, const QString& detail, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Game over");
    setModal(true);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    auto* titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 6);
    titleLabel->setFont(titleFont);

    auto* detailLabel = new QLabel(detail);
    detailLabel->setAlignment(Qt::AlignCenter);

    auto* rematchButton = new QPushButton("Rematch");
    auto* menuButton = new QPushButton("Back to menu");
    rematchButton->setDefault(true);

    auto* buttons = new QHBoxLayout;
    buttons->setSpacing(10);
    buttons->addWidget(menuButton);
    buttons->addWidget(rematchButton);

    layout->addWidget(titleLabel);
    layout->addWidget(detailLabel);
    layout->addLayout(buttons);

    connect(rematchButton, &QPushButton::clicked, this, [this]() {
        chosen = Rematch;
        accept();
    });

    connect(menuButton, &QPushButton::clicked, this, [this]() {
        chosen = Menu;
        accept();
    });
}


//todo fen format implement