//
// Created by Tobias Baert on 27/07/2022.
//

#include "Game.h"

#include <SFML/Graphics.hpp>

void Game::run() {
    while(view.isOpen()) {
        view.draw(board);
    }
}
