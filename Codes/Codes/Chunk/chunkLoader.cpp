#include <Codes/Chunk/chunkLoader.h>

#include <Codes/print.h>

//
#include <vector>

std::size_t IntPosHash::operator () (const IntPos &pos) const {
    std::size_t hash1 = std::hash<int>{}(pos.x);
    std::size_t hash2 = std::hash<int>{}(pos.y);
    std::size_t hash3 = std::hash<int>{}(pos.z);
    return (hash1 ^ (hash2 << 1)) ^ (hash3 << 1);
}

ChunkLoader::ChunkLoader() {
    auto terrainHeightSimplex = FastNoise::New<FastNoise::OpenSimplex2S>();
    terrainHeightFractal = FastNoise::New<FastNoise::FractalFBm>();
    terrainHeightFractal->SetSource(terrainHeightSimplex);
    terrainHeightFractal->SetOctaveCount(1);

    for (int x = -4; x <= 4; x++) {
        for (int z = -4; z <= 4; z++) {
            for (int y = -1; y <= 1; y++) {
                loadChunk(IntPos(x, y, z));
            }
        }
    }
}

void ChunkLoader::update() {
    for (auto &chunk: chunks) {
        if (!chunk.second->isMeshUpdateRequested()) {
            continue;
        }

        updateChunkMesh(chunk.first, chunk.second);
    }
}

const std::unordered_map<IntPos, std::unique_ptr<Chunk>, IntPosHash> &ChunkLoader::getChunkList() const {
    return chunks;
}

bool ChunkLoader::getBlock(IntPos blockPos) const {
    IntPos chunkPos = blockPos.getChunkPos();
    if (chunkLoaded(chunkPos)) {
        return chunks.at(chunkPos)->getBlock(blockPos.getBlockPosInChunk());
    }
    return false;
}

void ChunkLoader::breakBlock(IntPos blockPos) {
    IntPos chunkPos = blockPos.getChunkPos();
    if (!chunkLoaded(chunkPos)) {
        return;
    }

    chunks.at(chunkPos)->breakBlock(blockPos.getBlockPosInChunk());

    requestUpdateSideChunkMeshes(chunkPos);
}

void ChunkLoader::placeBlock(IntPos blockPos) {
    IntPos chunkPos = blockPos.getChunkPos();
    if (!chunkLoaded(chunkPos)) {
        return;
    }

    chunks.at(chunkPos)->placeBlock(blockPos.getBlockPosInChunk());

    requestUpdateSideChunkMeshes(chunkPos);
}

void ChunkLoader::requestUpdateSideChunkMeshes(IntPos chunkPos) {
    // Can be optimized: Only update mesh of chunk next to the placed block
    std::array<IntPos, 6> dirs = {
        IntPos( 0,  1,  0), // TOP
        IntPos( 0, -1,  0), // BOTTOM
        IntPos(-1,  0,  0), // LEFT
        IntPos( 1,  0,  0), // RIGHT
        IntPos( 0,  0,  1), // FORWARD
        IntPos( 0,  0, -1), // BACKWARD
    };
    for (IntPos dir: dirs) {
        IntPos sideChunkPos = chunkPos + dir;
        if (chunkLoaded(sideChunkPos)) {
            chunks.at(sideChunkPos)->meshUpdateRequested = true;
        }
    }
}

bool ChunkLoader::chunkLoaded(IntPos chunkPos) const {
    return chunks.find(chunkPos) != chunks.end();
}

void ChunkLoader::loadChunk(IntPos chunkPos) {
    auto getTerrainHeightIndex = [](int x, int z) {
        return z*CHUNK_WIDTH + x;
    };

    std::unique_ptr<Chunk> chunkPtr = std::make_unique<Chunk>();
    
    std::vector<float> terrainHeights(CHUNK_WIDTH*CHUNK_WIDTH);
    terrainHeightFractal->GenUniformGrid2D(terrainHeights.data(), 
                                            chunkPos.x*CHUNK_WIDTH, chunkPos.z*CHUNK_WIDTH, 
                                            CHUNK_WIDTH, CHUNK_WIDTH, 0.002f, 1);

    for (std::size_t i = 0; i < CHUNK_VOLUME; i++) {
        IntPos blockPos = Chunk::indexToPos(i);
        float terrainHeight = terrainHeights[getTerrainHeightIndex(blockPos.x, blockPos.z)];
        terrainHeight = (terrainHeight+1)/2;
        terrainHeight *= 16;
        if (blockPos.y < terrainHeight) {
            chunkPtr->blocks[i] = true;
        }
    }

    updateChunkMesh(chunkPos, chunkPtr);
    chunks.insert(std::make_pair(chunkPos, std::move(chunkPtr)));
}

void ChunkLoader::updateChunkMesh(IntPos chunkPos, std::unique_ptr<Chunk> &chunkPtr) {
    if (!chunkLoaded(chunkPos + IntPos( 0,  1,  0)) ||  // TOP
        !chunkLoaded(chunkPos + IntPos( 0, -1,  0)) ||  // BOTTOM
        !chunkLoaded(chunkPos + IntPos(-1,  0,  0)) ||  // LEFT
        !chunkLoaded(chunkPos + IntPos( 1,  0,  0)) ||  // RIGHT
        !chunkLoaded(chunkPos + IntPos( 0,  0,  1)) ||  // FORWARD
        !chunkLoaded(chunkPos + IntPos( 0,  0, -1))     // BACKWARD
    ) {
        return;
    }

    std::array<Chunk*, 6> sideChunks = {
        chunks.at(chunkPos + IntPos( 0,  1,  0)).get(), // TOP
        chunks.at(chunkPos + IntPos( 0, -1,  0)).get(), // BOTTOM
        chunks.at(chunkPos + IntPos(-1,  0,  0)).get(), // LEFT
        chunks.at(chunkPos + IntPos( 1,  0,  0)).get(), // RIGHT
        chunks.at(chunkPos + IntPos( 0,  0,  1)).get(), // FORWARD
        chunks.at(chunkPos + IntPos( 0,  0, -1)).get(), // BACKWARD
    };
    chunkPtr->updateMesh(sideChunks);
}
