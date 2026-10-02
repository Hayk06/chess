//
// Created by Hayk Derdzyan on 30.09.26.
//

#include "PromotionDialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

PromotionDialog::PromotionDialog(const Color color, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Promote to");
    setModal(true);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(12);

    auto* title = new QLabel(color == Color::WHITE ? "White pawn promotes to:"
                                                  : "Black pawn promotes to:");
    title->setAlignment(Qt::AlignCenter);

    auto* choices = new QHBoxLayout;
    choices->setSpacing(8);

    const struct { PieceType type; ushort glyph; const char* name; } options[] = {
        { PieceType::QUEEN,  0x265B, "Queen"  },
        { PieceType::ROOK,   0x265C, "Rook"   },
        { PieceType::BISHOP, 0x265D, "Bishop" },
        { PieceType::KNIGHT, 0x265E, "Knight" },
    };

    for (const auto& option : options) {
        auto* button = new QPushButton(QString(QChar(option.glyph)));
        button->setToolTip(option.name);
        button->setFixedSize(56, 56);

        QFont glyphFont = button->font();
        glyphFont.setPixelSize(30);
        button->setFont(glyphFont);

        connect(button, &QPushButton::clicked, this, [this, type = option.type]() {
            selection = type;
            accept();
        });

        choices->addWidget(button);
    }

    layout->addWidget(title);
    layout->addLayout(choices);
}
