//
// Created by Hayk Derdzyan on 30.09.26.
//

#ifndef CHESS_PROMOTIONDIALOG_H
#define CHESS_PROMOTIONDIALOG_H
#include <QDialog>

#include "../piece/PieceType.h"
#include "../shared/Color.h"

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(Color color, QWidget* parent = nullptr);

    PieceType chosen() const { return selection; }

private:
    PieceType selection = PieceType::QUEEN;   // fallback if dismissed
};

#endif //CHESS_PROMOTIONDIALOG_H
