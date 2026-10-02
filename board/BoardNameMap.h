//
// Created by Hayk Derdzyan on 10.09.26.
//

#ifndef CHESS_BOARDNAMEMAP_H
#define CHESS_BOARDNAMEMAP_H

#include <map>

struct BoardNameMap {
    std::map<int, char> map = {
        {0,'a'},
        {1,'b'},
        {2,'c'},
        {3,'d'},
        {4,'e'},
        {5,'f'},
        {6,'g'},
        {7,'h'},
    };
};

#endif //CHESS_BOARDNAMEMAP_H
