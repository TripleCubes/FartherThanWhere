#include <Codes/Chunk/chunk.h>

#include <vector>
#include <Codes/print.h>
#include <Codes/Types/vec3.h>
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

void Chunk::createFaceList(std::array<bool, CHUNK_VOLUME*6> &faceList, 
                            const std::array<Chunk*, 6> &sideChunks) const {
    std::array<IntPos, 6> dirs = {
        IntPos( 0,  1,  0), // TOP
        IntPos( 0, -1,  0), // BOTTOM
        IntPos(-1,  0,  0), // LEFT
        IntPos( 1,  0,  0), // RIGHT
        IntPos( 0,  0,  1), // FORWARD
        IntPos( 0,  0, -1), // BACKWARD
    };

    const auto isEdgeFace = [](IntPos blockPos, int faceDir) -> bool {
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

    const auto getSideChunkBlock = [&sideChunks](IntPos blockPos, int faceDir) -> bool {
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

    const auto getFaceIndex = [](int blockIndex, int faceDir) -> int {
        return blockIndex + faceDir * CHUNK_VOLUME;
    };

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
}

void Chunk::createSurfaceList(std::vector<Surface> &surfaceList, 
                                std::array<bool, CHUNK_VOLUME*6> &faceList) const {
    const auto getFaceIndex = [](int blockIndex, int faceDir) -> int {
        return blockIndex + faceDir * CHUNK_VOLUME;
    };

    const auto getCheckX = [](IntPos blockPos, int faceDir) -> int {
        switch (faceDir)
        {
        case 0: // TOP
        case 1: // BOTTOM
            return blockPos.x;
            break;

        case 2: // LEFT
        case 3: // RIGHT
            return blockPos.z;
            break;

        case 4: // FORWARD
        case 5: // BACKWARD
            return blockPos.x;
            break;
        
        default:
            return 0;
            break;
        }
    };

    const auto getCheckY = [](IntPos blockPos, int faceDir) -> int {
        switch (faceDir)
        {
        case 0: // TOP
        case 1: // BOTTOM
            return blockPos.z;
            break;

        case 2: // LEFT
        case 3: // RIGHT
            return blockPos.y;
            break;

        case 4: // FORWARD
        case 5: // BACKWARD
            return blockPos.y;
            break;
        
        default:
            return 0;
            break;
        }
    };

    const auto incX = [](int faceIndex) -> int {
        return faceIndex + CHUNK_WIDTH;
    };
    const auto incY = [](int faceIndex) -> int {
        return faceIndex + CHUNK_WIDTH*CHUNK_WIDTH;
    };
    const auto incZ = [](int faceIndex) -> int {
        return faceIndex + 1;
    };

    const auto decX = [](int faceIndex) -> int {
        return faceIndex - CHUNK_WIDTH;
    };
    const auto decZ = [](int faceIndex) -> int {
        return faceIndex - 1;
    };

    const auto right = [&incX, &incZ](int faceIndex, int faceDir) -> int {
        switch (faceDir)
        {
        case 0:
        case 1:
            return incX(faceIndex);
            break;

        case 2:
        case 3:
            return incZ(faceIndex);
            break;
        
        case 4:
        case 5:
            return incX(faceIndex);
            break;
        
        default:
            return 0;
            break;
        }
    };

    const auto left = [&decX, &decZ](int faceIndex, int faceDir) -> int {
        switch (faceDir)
        {
        case 0:
        case 1:
            return decX(faceIndex);
            break;

        case 2:
        case 3:
            return decZ(faceIndex);
            break;
        
        case 4:
        case 5:
            return decX(faceIndex);
            break;
        
        default:
            return 0;
            break;
        }
    };

    const auto down = [&incY, &incZ](int faceIndex, int faceDir) -> int {
        switch (faceDir)
        {
        case 0:
        case 1:
            return incZ(faceIndex);
            break;

        case 2:
        case 3:
            return incY(faceIndex);
            break;
        
        case 4:
        case 5:
            return incY(faceIndex);
            break;
        
        default:
            return 0;
            break;
        }
    };

    std::array<bool, CHUNK_VOLUME*6> faceCheckedList = {};

    for (int faceDir = 0; faceDir < 6; faceDir++) {
        for (int blockIndex = 0; blockIndex < CHUNK_VOLUME; blockIndex++) {
            int faceIndex = getFaceIndex(blockIndex, faceDir);
            if (!faceList[faceIndex] || faceCheckedList[faceIndex]) {
                continue;
            }

            IntPos blockPos = indexToPos(blockIndex);
            int surfaceW = 1;
            int surfaceH = 1;
            int checkStartX = getCheckX(blockPos, faceDir);
            int checkStartY = getCheckY(blockPos, faceDir);

            faceCheckedList[faceIndex] = true;
            int checkingIndex = right(faceIndex, faceDir);
            while (checkStartX + surfaceW < CHUNK_WIDTH
                    && faceList[checkingIndex] 
                    && !faceCheckedList[checkingIndex]) {
                surfaceW++;
                faceCheckedList[checkingIndex] = true;
                checkingIndex = right(checkingIndex, faceDir);
            }

            int checkingIndex_y = down(faceIndex, faceDir);
            while (true) {
                if (checkStartY + surfaceH >= CHUNK_WIDTH) {
                    goto stopYCheck;
                }

                int checkingIndex = checkingIndex_y;
                for (int i = 0; i < surfaceW; i++) {
                    if (!faceList[checkingIndex] || faceCheckedList[checkingIndex]) {
                        while (checkingIndex != checkingIndex_y) {
                            checkingIndex = left(checkingIndex, faceDir);
                            faceCheckedList[checkingIndex] = false;
                        }
                        goto stopYCheck;
                    }
                    faceCheckedList[checkingIndex] = true;
                    checkingIndex = right(checkingIndex, faceDir);
                }

                surfaceH++;
                checkingIndex_y = down(checkingIndex_y, faceDir);
            }
            stopYCheck:

            Chunk::Surface surface;
            surface.blockPos = blockPos;
            surface.faceDir = faceDir;
            surface.w = surfaceW;
            surface.h = surfaceH;
            surfaceList.push_back(surface);
        }
    }
}

void Chunk::createVerticies(std::vector<float> &verticies, 
                            std::vector<Surface> &surfaceList) const {
    for (Surface &surface: surfaceList) {
        std::vector<float> surfaceVerticies;
        Vec3 pos(surface.blockPos);
        float w = surface.w;
        float h = surface.h;

        switch (surface.faceDir) {
        case 0:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A TOP
                pos.x+w, pos.y+1, pos.z+h,  0,  1,  0,  1,  0, // C
                pos.x  , pos.y+1, pos.z+h,  0,  1,  0,  0,  0, // D

                pos.x  , pos.y+1, pos.z  ,  0,  1,  0,  0,  1, // A
                pos.x+w, pos.y+1, pos.z  ,  0,  1,  0,  1,  1, // B
                pos.x+w, pos.y+1, pos.z+h,  0,  1,  0,  1,  0, // C
            };
            break;
        
        case 1:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E BOTTOM
                pos.x  , pos.y  , pos.z+h,  0, -1,  0,  1,  0, // H
                pos.x+w, pos.y  , pos.z+h,  0, -1,  0,  0,  0, // G

                pos.x  , pos.y  , pos.z  ,  0, -1,  0,  1,  1, // E
                pos.x+w, pos.y  , pos.z+h,  0, -1,  0,  0,  0, // G
                pos.x+w, pos.y  , pos.z  ,  0, -1,  0,  0,  1, // F
            };
            break;

        case 2:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+h, pos.z  , -1,  0,  0,  0,  1, // A LEFT
                pos.x  , pos.y+h, pos.z+w, -1,  0,  0,  1,  1, // D
                pos.x  , pos.y  , pos.z+w, -1,  0,  0,  1,  0, // H

                pos.x  , pos.y+h, pos.z  , -1,  0,  0,  0,  1, // A
                pos.x  , pos.y  , pos.z+w, -1,  0,  0,  1,  0, // H
                pos.x  , pos.y  , pos.z  , -1,  0,  0,  0,  0, // E
            };
            break;

        case 3:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x+1, pos.y+h, pos.z  ,  1,  0,  0,  1,  1, // B RIGHT
                pos.x+1, pos.y  , pos.z+w,  1,  0,  0,  0,  0, // G
                pos.x+1, pos.y+h, pos.z+w,  1,  0,  0,  0,  1, // C

                pos.x+1, pos.y+h, pos.z  ,  1,  0,  0,  1,  1, // B
                pos.x+1, pos.y  , pos.z  ,  1,  0,  0,  1,  0, // F
                pos.x+1, pos.y  , pos.z+w,  1,  0,  0,  0,  0, // G
            };
            break;

        case 4:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+h, pos.z+1,  0,  0,  1,  0,  1, // D FORWARD
                pos.x+w, pos.y+h, pos.z+1,  0,  0,  1,  1,  1, // C
                pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H

                pos.x+w, pos.y+h, pos.z+1,  0,  0,  1,  1,  1, // C
                pos.x+w, pos.y  , pos.z+1,  0,  0,  1,  1,  0, // G
                pos.x  , pos.y  , pos.z+1,  0,  0,  1,  0,  0, // H
            };
            break;

        case 5:
            surfaceVerticies = {
//              Pos                         Normal      UV
                pos.x  , pos.y+h, pos.z  ,  0,  0, -1,  1,  1, // A BACKWARD
                pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
                pos.x+w, pos.y+h, pos.z  ,  0,  0, -1,  0,  1, // B

                pos.x+w, pos.y+h, pos.z  ,  0,  0, -1,  0,  1, // B
                pos.x  , pos.y  , pos.z  ,  0,  0, -1,  1,  0, // E
                pos.x+w, pos.y  , pos.z  ,  0,  0, -1,  0,  0, // F
            };
            break;
        
        default:
            break;
        }

        verticies.insert(verticies.end(), surfaceVerticies.begin(), surfaceVerticies.end());
    }
}

void Chunk::updateMesh(const std::array<Chunk*, 6> &sideChunks) {
    std::array<bool, CHUNK_VOLUME*6> faceList = {};
    createFaceList(faceList, sideChunks);

    std::vector<Surface> surfaceList;
    createSurfaceList(surfaceList, faceList);

    std::vector<float> verticies;
    createVerticies(verticies, surfaceList);
    
    mesh.set3d(verticies);
    chunkReady = true;
    meshUpdateRequested = false;
}

bool Chunk::isMeshUpdateRequested() const {
    return meshUpdateRequested;
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
