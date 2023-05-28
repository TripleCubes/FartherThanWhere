#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <memory>
#include <Codes/view.h>
#include <Codes/Types/intPos.h>
#include <Codes/Chunk/chunk.h>

//
#include <FastNoise/FastNoise.h>

class ChunkLoader {
    friend void View::draw() const;

public:
    ChunkLoader();

private:
    struct IntPosHash {
        std::size_t operator () (const IntPos &pos) const;
    };
    std::unordered_map<IntPos, std::unique_ptr<Chunk>, IntPosHash> chunks;

    void loadChunk(IntPos chunkPos);
    bool chunkLoaded(IntPos chunkPos) const;

    //
    FastNoise::SmartNode<FastNoise::FractalFBm> terrainHeightFractal;

    void updateChunkMesh(IntPos chunkPos, std::unique_ptr<Chunk> &chunkPtr);
};

#endif