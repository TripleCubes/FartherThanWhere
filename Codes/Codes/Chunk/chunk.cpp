#include <Codes/Chunk/chunk.h>

#include <vector>
#include <Codes/print.h>
#include <Codes/Types/intPos.h>
#include <Codes/Types/vec3.h>

Chunk::Chunk() {
    blocks.fill(true);
}

int Chunk::posToIndex(IntPos pos) const {
    if (pos.x < 0 || pos.x >= CHUNK_SIZE
    || pos.y < 0 || pos.y >= CHUNK_SIZE
    || pos.z < 0 || pos.z >= CHUNK_SIZE) {
        PRINTLN("Chunk::posToIndex(): Invalid block pos");
        return 0;
    }
    return pos.y * CHUNK_SIZE*CHUNK_SIZE + pos.x * CHUNK_SIZE + pos.z;
}

IntPos Chunk::indexToPos(int index) const {
    if (index < 0 || index >= CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE) {
        PRINTLN("Chunk::indexToPos(): Invalid index");
        return IntPos(0, 0, 0);
    }
    IntPos pos(0, 0, 0);
    pos.z = index % CHUNK_SIZE;
    index /= CHUNK_SIZE;
    pos.x = index % CHUNK_SIZE;
    index /= CHUNK_SIZE;
    pos.y = index;
    return pos;
}

void Chunk::placeBlock(IntPos pos) {
    blocks[posToIndex(pos)] = true;
    meshUpdateRequested = true;
}

void Chunk::breakBlock(IntPos pos) {
    blocks[posToIndex(pos)] = false;
    meshUpdateRequested = true;
}

bool Chunk::getBlock(IntPos pos) const {
    return blocks[posToIndex(pos)];
}

void Chunk::updateMesh() {
    if (!meshUpdateRequested) {
        return;
    }

    std::vector<float> verticies;

    for (std::size_t i = 0; i < blocks.size(); i++) {
        if (!blocks[i]) {
            continue;
        }

        Vec3 pos(indexToPos(i));
        std::vector<float> blockVerticies = {
//          Pos                         Normal      UV
            pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A
            pos.x+1, pos.y+1, pos.z+1,  0,  1,  0,  1,  0, // C
            pos.x  , pos.y+1, pos.z+1,  0,  1,  0,  0,  0, // D

            pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A
            pos.x+1, pos.y+1, pos.z  ,  0,  1,  0,  1,  1, // B
            pos.x+1, pos.y+1, pos.z+1,  0,  1,  0,  1,  0, // C

            pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E
            pos.x  , pos.y  , pos.z+1,  0, -1,  0,  1,  0, // H
            pos.x+1, pos.y  , pos.z+1,  0, -1,  0,  0,  0, // G

            pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E
            pos.x+1, pos.y  , pos.z+1,  0, -1,  0,  0,  0, // G
            pos.x+1, pos.y  , pos.z  ,  0, -1,  0,  0,  1, // F

            pos.x  , pos.y+1, pos.z  , -1,  0,  0,  0,  1, // A
            pos.x  , pos.y+1, pos.z+1, -1,  0,  0,  1,  1, // D
            pos.x  , pos.y  , pos.z+1, -1,  0,  0,  1,  0, // H

            pos.x  , pos.y+1, pos.z  , -1,  0,  0,  0,  1, // A
            pos.x  , pos.y  , pos.z+1, -1,  0,  0,  1,  0, // H
            pos.x  , pos.y  , pos.z  , -1,  0,  0,  0,  0, // E

            pos.x+1, pos.y+1, pos.z  ,  1,  0,  0,  1,  1, // B
            pos.x+1, pos.y  , pos.z+1,  1,  0,  0,  0,  0, // G
            pos.x+1, pos.y+1, pos.z+1,  1,  0,  0,  0,  1, // C

            pos.x+1, pos.y+1, pos.z  ,  1,  0,  0,  1,  1, // B
            pos.x+1, pos.y  , pos.z  ,  1,  0,  0,  1,  0, // F
            pos.x+1, pos.y  , pos.z+1,  1,  0,  0,  0,  0, // G

            pos.x  , pos.y+1, pos.z+1,  0,  0,  1,  0,  1, // D
            pos.x+1, pos.y+1, pos.z+1,  0,  0,  1,  1,  1, // C
            pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H

            pos.x+1, pos.y+1, pos.z+1,  0,  0,  1,  1,  1, // C
            pos.x+1, pos.y  , pos.z+1,  0,  0,  1,  1,  0, // G
            pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H

            pos.x  , pos.y+1, pos.z  ,  0,  0, -1,  1,  1, // A
            pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
            pos.x+1, pos.y+1, pos.z  ,  0,  0, -1,  0,  1, // B

            pos.x+1, pos.y+1, pos.z  ,  0,  0, -1,  0,  1, // B
            pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
            pos.x+1, pos.y  , pos.z  ,  0,  0, -1,  0,  0, // F
        };

        verticies.insert(verticies.end(), blockVerticies.begin(), blockVerticies.end());
    }

    mesh.set3d(verticies);
    meshInitialized = true;
    meshUpdateRequested = false;
}

void Chunk::draw() const {
    if (!meshInitialized) {
        return;
    }

    mesh.draw();
}

Chunk::~Chunk() {
    mesh.release();
}
