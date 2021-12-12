#pragma once

#include <cstdint>

#include <mrld/mrld.h>

class GameLoop
{
    typedef uint64_t (*step_callback)(uint64_t dt_micros, uint64_t accumulator);
public:
    GameLoop(uint32_t tps, step_callback cb);
    void start();
    void stop();
    inline uint32_t get_last_fps() const { return _last_fps; }

private:
    uint32_t _tps;
    uint32_t _fps;
    step_callback _callback;
    bool _running;

    uint32_t _last_fps;
    uint64_t _dt;

    mrld::Timer _timer;
};


