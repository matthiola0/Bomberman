#include "GameWindow.h"

int main(int argc, char *argv[])
{
    GameWindow *BoboGame= new GameWindow();

    BoboGame->game_play();

    delete BoboGame;
    return 0;
}
