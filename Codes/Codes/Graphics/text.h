#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>

const int FONT_HEIGHT = 24;

class Text {
public:
    struct Character
    {
        unsigned int textureId;
        float w = 0;
        float h = 0;
        float bearingX = 0;
        float bearingY = 0;
        float advance = 0;
        float lineHeight = 0;
    };
    
    static void init();
    static Character getCharacter(char characterCode);
    static void release();

private:
    static FT_Library library;
    static FT_Face face;

    static std::vector<Character> characters;
    static int firstCharacterCode;
};

#endif
