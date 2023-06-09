#include <Codes/game.h>

#include <Codes/print.h>
#include <Codes/world.h>

Game::Game(World &world): chunkLoader(world.chunkLoader) {}

void Game::update() {
    controls.update();

    player.update();
    camera.update();
    view.update();

    intervals.updateIntervals();
}

void Game::draw() const {
    view.draw();
}

Game::~Game() {
}
