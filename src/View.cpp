//
// Created by Tobias Baert on 27/07/2022.
//

#include "View.h"

const int View::DEFAULT_WINDOW_SIZE = 480;
const int View::DEFAULT_CELL_SIZE = DEFAULT_WINDOW_SIZE / 6;

View::View() : window(sf::VideoMode(DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE), "Pentago") {}

bool View::isOpen() {
    return window.isOpen();
}
