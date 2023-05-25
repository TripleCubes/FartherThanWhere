#include <Codes/game.h>

#include <Codes/print.h>
#include <Codes/world.h>

Game::Game(World &world): chunkLoader(world.chunkLoader) {}

void Game::update() {
    controls.update();

    view.draw();

    intervals.updateIntervals();
}

Game::~Game() {
    PRINTLN("game deconstructed");
}
