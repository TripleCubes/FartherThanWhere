#include <Codes/world.h>

#include <Codes/Debug/print.h>

void World::update() {
    chunkLoader.update();
}

World::~World() {
}
