//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_SIDE_H
#define CHESS_SIDE_H

#include <chrono>
#include <string>
#include <vector>

#include "../shared/Color.h"
#include "../piece/PieceType.h"

class Side{
private:
    Color color;
    std::string name;
    std::vector<PieceType> captured;
    std::chrono::milliseconds remaining;
public:
    Side();
    Side(Color color, std::string name, std::chrono::milliseconds remaining);

    void randomSide();
    void setColor(Color side);
    Color getColor() const;

    void setName(std::string value);
    const std::string& getName() const;

    void addCapture(PieceType type);
    const std::vector<PieceType>& getCaptured() const;
    int get_captured_points() const;

    void setRemaining(std::chrono::milliseconds value);
    std::chrono::milliseconds getRemaining() const;
};

#endif //CHESS_SIDE_H

//todo discuss / thnnk about side usage , history storage , save and export
