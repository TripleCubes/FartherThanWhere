#include <Codes/world.h>

#include <Codes/print.h>

World::~World() {
    PRINTLN("world deconstructed");
}

void World::update() {
    chunkLoader.update();
}
