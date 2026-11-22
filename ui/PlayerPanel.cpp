//
// Created by Hayk Derdzyan on 13.09.26.
//

#include "PlayerPanel.h"
#include <QHBoxLayout>
#include <QLabel>

PlayerPanel::PlayerPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 6, 10, 6);
    layout->setSpacing(12);

    capturedLabel = new QLabel;
    capturedLabel->setStyleSheet("color: #666666;");
    QFont capturedFont = capturedLabel->font();
    capturedFont.setPixelSize(19);
    capturedLabel->setFont(capturedFont);

    clockLabel = new QLabel("00:00");
    QFont clockFont = clockLabel->font();
    clockFont.setPointSize(clockFont.pointSize() + 4);
    clockFont.setBold(true);
    clockLabel->setFont(clockFont);
    clockLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    clockLabel->setStyleSheet("background: #2b2b2b; color: #f0f0f0; padding: 2px 10px; border-radius: 4px;");

    layout->addWidget(capturedLabel, 1);
    layout->addWidget(clockLabel);

    setCaptured(QString(), 0);
}

void PlayerPanel::setClock(const QString& clock)
{
    clockLabel->setText(clock);
}

void PlayerPanel::setCaptured(const QString& pieces, int points)
{
    QString text = pieces;
    if (points > 0) {
        text += QString("  +%1").arg(points);
    }
    capturedLabel->setText(text);
}
