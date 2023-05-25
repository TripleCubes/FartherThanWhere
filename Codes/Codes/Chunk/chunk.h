#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <Codes/GraphicTypes/mesh.h>
class IntPos;

const int CHUNK_SIZE = 16;

class Chunk {
public:
    Chunk();

    void placeBlock(IntPos pos);
    void breakBlock(IntPos pos);
    bool getBlock(IntPos pos) const;

    void updateMesh();
    void draw() const;

    ~Chunk();
    
private:
    std::array<bool, CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE> blocks = {};
    Mesh mesh;
    bool meshInitialized = false;
    bool meshUpdateRequested = true;

    int posToIndex(IntPos pos) const;
    IntPos indexToPos(int index) const;
};

#endif
