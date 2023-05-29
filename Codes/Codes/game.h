#ifndef GAME_H
#define GAME_H

#include <Codes/controls.h>
#include <Codes/settings.h>
#include <Codes/view.h>
#include <Codes/Time/intervals.h>
#include <Codes/Entities/player.h>
class World;
class ChunkLoader;

class Game {
public:
    Game(World &world);
    void update();
    ~Game();
    
private:
    ChunkLoader &chunkLoader;

    Controls controls = Controls(settings, view);
    Settings settings;
    View view = View(settings, chunkLoader);
    Intervals intervals;
    Player player;
};

#endif
