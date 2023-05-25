#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <memory>
#include <Codes/view.h>
#include <Codes/Types/intPos.h>
#include <Codes/Chunk/chunk.h>

class ChunkLoader {
    friend void View::draw() const;

public:
    ChunkLoader();

private:
    struct IntPosHash {
        std::size_t operator () (const IntPos &pos) const;
    };
    std::unordered_map<IntPos, std::unique_ptr<Chunk>, IntPosHash> chunks;
};

#endif
