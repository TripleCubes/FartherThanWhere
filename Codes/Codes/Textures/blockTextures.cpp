#include <Codes/Textures/blockTextures.h>

ArrayTexture GameTextures::BlockTextures::arrayTexture_blocks;

void GameTextures::BlockTextures::init() {
    arrayTexture_blocks.init("Textures/Blocks/blocks.png", 8, 8);
}

int GameTextures::BlockTextures::getTextureIndex(int blockType, int faceDir) {
    const int TOP = 0;
    const int BOTTOM = 1;
    // const int LEFT = 2;
    // const int RIGHT = 3;
    // const int FORWARD = 4;
    // const int BACKWARD = 5;

    switch (blockType)
    {
    case BLOCKTYPE_STONE:
        return 0;
        break;

    case BLOCKTYPE_DIRT:
        return 1;
        break;

    case BLOCKTYPE_GRASS:
        if (faceDir == TOP) {
            return 3;
        }
        if (faceDir == BOTTOM) {
            return 1;
        }
        return 2;
        break;

    case BLOCKTYPE_LOG:
        if (faceDir == TOP || faceDir == BOTTOM) {
            return 5;
        }
        return 4;
        break;

    case BLOCKTYPE_LEAF:
        return 6;
        break;

    case BLOCKTYPE_GLASS:
        return 7;
        break;
    
    default:
        return 0;
        break;
    }
}

unsigned int GameTextures::BlockTextures::getBlocksTextureId() {
    return arrayTexture_blocks.getTextureId();
}

void GameTextures::BlockTextures::release() {
    arrayTexture_blocks.release();
}
