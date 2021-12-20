#include "src/game.h"

int main(int argc, char *argv[])
{
    Game &game = Game::get_instance();
    game.start();
    game.cleanup();
    return 0;
}
