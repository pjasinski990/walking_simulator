#include <thread>

#include "game_loop.h"

GameLoop::GameLoop(uint32_t tps, GameLoop::step_callback cb)
        : _tps{tps}
        , _callback{cb}
        , _running{false}
        , _last_fps{0u}
        , _dt{1000000u / _tps}
{
    /* no-op */
}

void GameLoop::start()
{
    _running = true;
    uint64_t accumulator = 0u;
    uint64_t current_time = _timer.get_elapsed_micros();
    uint64_t fps_timer = 0u;
    uint32_t fps = 0;

    while (_running) {
        uint64_t new_time = _timer.get_elapsed_micros();
        uint64_t frame_time = new_time - current_time;
        current_time = new_time;
        accumulator += frame_time;
        accumulator = _callback(_dt, accumulator);

        if (_timer.get_elapsed_millis() - fps_timer > 1000u) {
            fps_timer = _timer.get_elapsed_millis();
            std::cout << fps << std::endl;
            _last_fps = fps;
            fps = 0;
        }
        ++fps;
    }
}

void GameLoop::stop()
{
    _running = false;
}
