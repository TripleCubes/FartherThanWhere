#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <memory>
#include <Codes/GraphicTypes/mesh.h>
class IntPos;

const int CHUNK_WIDTH = 16;
const int CHUNK_VOLUME = CHUNK_WIDTH*CHUNK_WIDTH*CHUNK_WIDTH;

class Chunk {
    friend class ChunkLoader;

public:
    Chunk();

    void placeBlock(IntPos pos);
    void breakBlock(IntPos pos);
    bool getBlock(IntPos pos) const;

    void updateMesh(const std::array<Chunk*, 6> &sideChunks);

    void draw() const;

    ~Chunk();
    
private:
    std::array<bool, CHUNK_VOLUME> blocks = {};
    Mesh mesh;
    bool chunkReady = false;
    bool meshUpdateRequested = true;

    static int posToIndex(IntPos pos);
    static IntPos indexToPos(int index);
};

#endif
