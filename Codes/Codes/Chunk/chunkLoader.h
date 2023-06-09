#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <Codes/Types/intPos.h>
#include <Codes/Chunk/chunk.h>

//
#include <FastNoise/FastNoise.h>

struct IntPosHash {
    std::size_t operator () (const IntPos &pos) const;
};

class ChunkLoader {    
public:
    ChunkLoader();

    void update();

    const std::unordered_map<IntPos, std::unique_ptr<Chunk>, IntPosHash> &getChunkList() const;

    bool getBlock(IntPos blockPos) const;
    void placeBlock(IntPos blockPos);
    void breakBlock(IntPos blockPos);

    struct ChunkLoadRequest {
        IntPos chunkPos;
        int loadDistance = 1;
    };
    void requestChunkLoad(ChunkLoadRequest request);

private:
    int chunkLoadCapPerFrame = 3;
    
    std::unordered_map<IntPos, std::unique_ptr<Chunk>, IntPosHash> chunks;

    void loadChunk(IntPos chunkPos);
    void loadChunkArea(ChunkLoadRequest request);
    bool chunkShouldBeUnloaded(IntPos chunkPos);
    bool chunkLoaded(IntPos chunkPos) const;
    void requestUpdateSideChunkMeshes(IntPos chunkPos);

    std::vector<ChunkLoadRequest> chunkLoadRequestList; 

    //
    FastNoise::SmartNode<FastNoise::FractalFBm> terrainHeightFractal;

    void updateChunkMesh(IntPos chunkPos, std::unique_ptr<Chunk> &chunkPtr);
};

#endif
