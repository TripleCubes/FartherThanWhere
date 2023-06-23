#ifndef BLOCKTEXTURES_H
#define BLOCKTEXTURES_H

#include <Codes/Chunk/block.h>
#include <Codes/GraphicTypes/arrayTexture.h>

namespace GameTextures {
    class BlockTextures {
    public:
        static void init();

        static int getTextureIndex(int blockType, int faceDir);
        static unsigned int getBlocksTextureId();

        static void release();

    private:
        static ArrayTexture arrayTexture_blocks;
    };
}

#endif
