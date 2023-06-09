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
    void draw() const;
    ~Game();
    
private:
    ChunkLoader &chunkLoader;

    Controls controls = Controls(settings, camera, chunkLoader, player);
    Settings settings;
    View view = View(settings, camera, chunkLoader, player);
    Intervals intervals;
    Player player = Player(settings, chunkLoader);
    Camera camera = Camera(chunkLoader, player);
};

#endif
