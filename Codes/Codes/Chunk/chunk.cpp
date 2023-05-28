#include <Codes/Chunk/chunk.h>

#include <vector>
#include <Codes/print.h>
#include <Codes/Types/vec3.h>
#include <Codes/Types/vec2.h>
#include <Codes/Chunk/chunkLoader.h>

Chunk::Chunk() {}

int Chunk::posToIndex(IntPos pos) {
    if (pos.x < 0 || pos.x >= CHUNK_WIDTH
    || pos.y < 0 || pos.y >= CHUNK_WIDTH
    || pos.z < 0 || pos.z >= CHUNK_WIDTH) {
        PRINTLN("Chunk::posToIndex(): Invalid block pos");
        return 0;
    }
    return pos.y * CHUNK_WIDTH*CHUNK_WIDTH + pos.x * CHUNK_WIDTH + pos.z;
}

IntPos Chunk::indexToPos(int index) {
    if (index < 0 || index >= CHUNK_VOLUME) {
        PRINTLN("Chunk::indexToPos(): Invalid index");
        return IntPos(0, 0, 0);
    }
    IntPos pos(0, 0, 0);
    pos.z = index % CHUNK_WIDTH;
    index /= CHUNK_WIDTH;
    pos.x = index % CHUNK_WIDTH;
    index /= CHUNK_WIDTH;
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

void Chunk::updateMesh(const std::array<Chunk*, 6> &sideChunks) {
    std::array<IntPos, 6> dirs = {
        IntPos( 0,  1,  0), // TOP
        IntPos( 0, -1,  0), // BOTTOM
        IntPos(-1,  0,  0), // LEFT
        IntPos( 1,  0,  0), // RIGHT
        IntPos( 0,  0,  1), // FORWARD
        IntPos( 0,  0, -1), // BACKWARD
    };

    auto isEdgeFace = [](IntPos blockPos, int faceDir) -> bool {
        switch (faceDir) {
        case 0: // TOP
            return blockPos.y == CHUNK_WIDTH-1;
            break;

        case 1: // BOTTOM
            return blockPos.y == 0;
            break;
        
        case 2: // LEFT
            return blockPos.x == 0;
            break;

        case 3: // RIGHT
            return blockPos.x == CHUNK_WIDTH-1;
            break;

        case 4: // FORWARD
            return blockPos.z == CHUNK_WIDTH-1;
            break;

        case 5: // BACKWARD
            return blockPos.z == 0;
            break;
        
        default:
            return false;
            break;
        }
    };

    auto getSideChunkBlock = [&sideChunks](IntPos blockPos, int faceDir) -> bool {
        switch (faceDir) {
        case 0: // TOP
            return sideChunks[0]->getBlock(IntPos(blockPos.x, 0, blockPos.z));
            break;

        case 1: // BOTTOM
            return sideChunks[1]->getBlock(IntPos(blockPos.x, CHUNK_WIDTH-1, blockPos.z));
            break;

        case 2: // LEFT
            return sideChunks[2]->getBlock(IntPos(CHUNK_WIDTH-1, blockPos.y, blockPos.z));
            break;

        case 3: // RIGHT
            return sideChunks[3]->getBlock(IntPos(0, blockPos.y, blockPos.z));
            break;

        case 4: // FORWARD
            return sideChunks[4]->getBlock(IntPos(blockPos.x, blockPos.y, 0));
            break;

        case 5: // BACKWARD
            return sideChunks[5]->getBlock(IntPos(blockPos.x, blockPos.y, CHUNK_WIDTH-1));
            break;
        
        default:
            return false;
            break;
        }
    };

    auto getFaceIndex = [](int blockIndex, int faceDir) -> int {
        return blockIndex + faceDir * CHUNK_VOLUME;
    };
    auto faceIndexToFaceDir = [](int faceIndex) -> int {
        return faceIndex / CHUNK_VOLUME;
    };
    auto faceIndexToBlockIndex = [](int faceIndex) -> int {
        return faceIndex % CHUNK_VOLUME;
    };
    std::array<bool, CHUNK_VOLUME*6> faceList = {};

    for (int blockIndex = 0; blockIndex < CHUNK_VOLUME; blockIndex++) {
        if (!blocks[blockIndex]) {
            continue;
        }
        IntPos blockPos = indexToPos(blockIndex);
        for (int faceDir = 0; faceDir < 6; faceDir++) {
            if (!isEdgeFace(blockPos, faceDir)) {
                if (!getBlock(blockPos + dirs[faceDir])) {
                    faceList[getFaceIndex(blockIndex, faceDir)] = true;
                }
            } else {
                if (!getSideChunkBlock(blockPos, faceDir)) {
                    faceList[getFaceIndex(blockIndex, faceDir)] = true;
                }
            }
        }
    }



    struct Surface {
        int w = 0;
        int h = 0;
        int faceDir = 0;
        IntPos blockPos = IntPos(0, 0, 0);
    };
    std::vector<Surface> surfaces;

    for (int faceIndex = 0; faceIndex < CHUNK_VOLUME*6; faceIndex++) {
        if (!faceList[faceIndex]) {
            continue;
        }
        int blockIndex = faceIndexToBlockIndex(faceIndex);
        int faceDir = faceIndexToFaceDir(faceIndex);
        Surface surface;
        surface.faceDir = faceDir;
        surface.blockPos = indexToPos(blockIndex);
        surfaces.push_back(surface);
    }



    std::vector<float> verticies;

    for (Surface &surface: surfaces) {
        std::vector<float> surfaceVerticies;
        Vec3 pos(surface.blockPos);

        switch (surface.faceDir) {
        case 0:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A TOP
                pos.x+1, pos.y+1, pos.z+1,  0,  1,  0,  1,  0, // C
                pos.x  , pos.y+1, pos.z+1,  0,  1,  0,  0,  0, // D

                pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A
                pos.x+1, pos.y+1, pos.z  ,  0,  1,  0,  1,  1, // B
                pos.x+1, pos.y+1, pos.z+1,  0,  1,  0,  1,  0, // C
            };
            break;
        
        case 1:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E BOTTOM
                pos.x  , pos.y  , pos.z+1,  0, -1,  0,  1,  0, // H
                pos.x+1, pos.y  , pos.z+1,  0, -1,  0,  0,  0, // G

                pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E
                pos.x+1, pos.y  , pos.z+1,  0, -1,  0,  0,  0, // G
                pos.x+1, pos.y  , pos.z  ,  0, -1,  0,  0,  1, // F
            };
            break;

        case 2:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+1, pos.z  , -1,  0,  0,  0,  1, // A LEFT
                pos.x  , pos.y+1, pos.z+1, -1,  0,  0,  1,  1, // D
                pos.x  , pos.y  , pos.z+1, -1,  0,  0,  1,  0, // H

                pos.x  , pos.y+1, pos.z  , -1,  0,  0,  0,  1, // A
                pos.x  , pos.y  , pos.z+1, -1,  0,  0,  1,  0, // H
                pos.x  , pos.y  , pos.z  , -1,  0,  0,  0,  0, // E
            };
            break;

        case 3:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x+1, pos.y+1, pos.z  ,  1,  0,  0,  1,  1, // B RIGHT
                pos.x+1, pos.y  , pos.z+1,  1,  0,  0,  0,  0, // G
                pos.x+1, pos.y+1, pos.z+1,  1,  0,  0,  0,  1, // C

                pos.x+1, pos.y+1, pos.z  ,  1,  0,  0,  1,  1, // B
                pos.x+1, pos.y  , pos.z  ,  1,  0,  0,  1,  0, // F
                pos.x+1, pos.y  , pos.z+1,  1,  0,  0,  0,  0, // G
            };
            break;

        case 4:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+1, pos.z+1,  0,  0,  1,  0,  1, // D FORWARD
                pos.x+1, pos.y+1, pos.z+1,  0,  0,  1,  1,  1, // C
                pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H

                pos.x+1, pos.y+1, pos.z+1,  0,  0,  1,  1,  1, // C
                pos.x+1, pos.y  , pos.z+1,  0,  0,  1,  1,  0, // G
                pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H
            };
            break;

        case 5:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+1, pos.z  ,  0,  0, -1,  1,  1, // A BACKWARD
                pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
                pos.x+1, pos.y+1, pos.z  ,  0,  0, -1,  0,  1, // B

                pos.x+1, pos.y+1, pos.z  ,  0,  0, -1,  0,  1, // B
                pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
                pos.x+1, pos.y  , pos.z  ,  0,  0, -1,  0,  0, // F
            };
            break;
        
        default:
            break;
        }

        verticies.insert(verticies.end(), surfaceVerticies.begin(), surfaceVerticies.end());
    }

    mesh.set3d(verticies);
    chunkReady = true;
    meshUpdateRequested = false;
}

void Chunk::draw() const {
    if (!chunkReady) {
        return;
    }

    mesh.draw();
}

Chunk::~Chunk() {
    if (chunkReady) {
        mesh.release();
    }
}
