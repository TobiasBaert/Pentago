//
// Created by Tobias Baert on 27/07/2022.
//

#ifndef PENTAGO_VIEW_H_DATE_27_07_2022_TIME_13_27
#define PENTAGO_VIEW_H_DATE_27_07_2022_TIME_13_27


#include <SFML/Graphics/RenderWindow.hpp>
#include "EnumBoard.h"

class View {
public:
    View();

    bool isOpen();

    void draw(const EnumBoard& board);

protected:

private:
    sf::RenderWindow window;
    static const int DEFAULT_WINDOW_SIZE;
    static const int DEFAULT_CELL_SIZE;
};


#endif //PENTAGO_VIEW_H_DATE_27_07_2022_TIME_13_27
