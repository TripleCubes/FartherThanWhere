#include <Codes/world.h>

#include <Codes/print.h>

void World::update() {
    chunkLoader.update();
}

World::~World() {
}
