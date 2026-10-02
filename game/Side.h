//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_SIDE_H
#define CHESS_SIDE_H

#include "../shared/Color.h"

class Side{
private:
    Color color;
    int captured_points;
public:
    Side();
    void randomSide();
    void setColor(Color side);
    Color getColor() const;
    void set_captured_points(int points);
    int get_captured_points() const;
};

#endif //CHESS_SIDE_H

//todo discuss / thnnk about side usage , history storage , save and export
