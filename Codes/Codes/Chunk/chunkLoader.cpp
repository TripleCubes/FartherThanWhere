#include <Codes/Chunk/chunkLoader.h>

#include <Codes/print.h>

std::size_t ChunkLoader::IntPosHash::operator () (const IntPos &pos) const {
    std::size_t hash1 = std::hash<int>{}(pos.x);
    std::size_t hash2 = std::hash<int>{}(pos.y);
    std::size_t hash3 = std::hash<int>{}(pos.z);
    return (hash1 ^ (hash2 << 1)) ^ (hash3 << 1);
}

ChunkLoader::ChunkLoader() {
    // for (int x = -3; x <= 3; x++) {
    //     for (int z = -3; z <= 3; z++) {
    //         Chunk chunk;
    //         chunk.updateMesh();
    //         chunks.insert(std::make_pair(IntPos(x, 0, z), chunk));
    //     }
    // }
    std::unique_ptr<Chunk> chunkPtr = std::make_unique<Chunk>();
    chunkPtr->updateMesh();
    chunks.insert(std::make_pair(IntPos(0, 0, 0), std::move(chunkPtr)));
}
