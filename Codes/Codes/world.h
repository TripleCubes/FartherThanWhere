#ifndef WORLD_H
#define WORLD_H

#include <Codes/Chunk/chunkLoader.h>
#include <Codes/game.h>

class World {
    friend Game::Game(World &world);

public:
    ~World();

private:
    ChunkLoader chunkLoader;
};

#endif
