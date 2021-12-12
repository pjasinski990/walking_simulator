#include <mrld/mrld.h>

#include "src/game.h"

void handle_keys(mrld::KeyboardHandler& handler, mrld::FPSCamera &cam, float dt);

int main(int argc, char *argv[])
{
    Game &game = Game::get_instance();
    game.start();
    game.cleanup();
    return 0;
}
