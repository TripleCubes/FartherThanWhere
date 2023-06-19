#include <Codes/game.h>

#include <Codes/world.h>
#include <Codes/Chunk/chunkLoader.h>
#include <Codes/Types/intPos.h>
#include <Codes/print.h>

Game::Game(World &world): chunkLoader(world.chunkLoader) {
    settings.loadDistance = 4;
}

void Game::update() {
    controls.update();

    ChunkLoader::ChunkLoadRequest chunkLoadRequest;
    chunkLoadRequest.chunkPos = IntPos(player.getPos()).getChunkPos();
    chunkLoadRequest.loadDistance = settings.getLoadDistance();
    chunkLoader.requestChunkLoad(chunkLoadRequest);

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
