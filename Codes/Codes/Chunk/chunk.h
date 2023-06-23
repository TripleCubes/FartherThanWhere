#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <memory>
#include <Codes/GraphicTypes/mesh.h>
#include <Codes/Types/vec2.h>
#include <Codes/Types/intPos.h>

//
#include <Codes/Chunk/block.h>

const int CHUNK_WIDTH = 16;
const int CHUNK_VOLUME = CHUNK_WIDTH*CHUNK_WIDTH*CHUNK_WIDTH;

class Chunk {
    friend class ChunkLoader;

public:
    Chunk();

    void placeBlock(IntPos pos, int blockType);
    void breakBlock(IntPos pos);
    int getBlock(IntPos pos) const;

    void updateMesh(const std::array<Chunk*, 6> &sideChunks);
    bool isMeshUpdateRequested() const;

    void draw() const;

    ~Chunk();
    
private:
    std::array<int, CHUNK_VOLUME> blocks = {};
    Mesh mesh;
    bool chunkReady = false;
    bool meshUpdateRequested = true;

    static int posToIndex(IntPos pos);
    static IntPos indexToPos(int index);

    void createFaceList(std::array<bool, CHUNK_VOLUME*6> &faceList, 
                            const std::array<Chunk*, 6> &sideChunks) const;
    struct Surface {
        int textureIndex;
        Vec2 uv;
        int w = 0;
        int h = 0;
        int faceDir = 0;
        IntPos blockPos;
    };
    void createSurfaceList(std::vector<Surface> &surfaceList, 
                            std::array<bool, CHUNK_VOLUME*6> &faceList) const;
    void createVerticies(std::vector<float> &verticies, 
                            std::vector<Surface> &surfaceList) const;
};

#endif
