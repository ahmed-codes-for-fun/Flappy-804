#include <iostream>
#include"Game.h"
#include<vector>

using namespace sf;

int main()
{
    srand(time(NULL));
    Game game;

    game.playMusic();
    ///Game Loop///
    while (game.getWindow()->isOpen())
    {
        game.update();
        game.render();
    }

    return 0;
}