#ifndef BLOCKRAYCAST_H
#define BLOCKRAYCAST_H

#include <Codes/Types/intPos.h>
class Vec3;
class ChunkLoader;

class BlockRaycast {
public:
    BlockRaycast(const ChunkLoader &chunkLoader);

    struct Result {
        bool found = false;
        bool selectedType = false;
        IntPos selectedPos;

        bool hasPlacingPos = false;
        IntPos placingPos;
    };

    Result cast(Vec3 from, Vec3 dir, float range);

private:
    const ChunkLoader &chunkLoader;
};

#endif
