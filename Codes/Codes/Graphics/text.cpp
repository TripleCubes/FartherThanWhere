#include <Codes/Graphics/text.h>

#include <glad/glad.h>

#include <Codes/Debug/print.h>

FT_Library Text::library;
FT_Face Text::face;

std::vector<Text::Character> Text::characters;
int Text::firstCharacterCode = 32;

void Text::init() {
    if (FT_Init_FreeType(&library))
    {
        PRINTLN("Couldnt init freetype");
    }
    
    if (FT_New_Face(library, "Fonts/Barlow_Semi_Condensed/BarlowSemiCondensed-SemiBold.ttf", 0, &face))
    {
        PRINTLN("couldnt load font file");
    }

    FT_Set_Pixel_Sizes(face, 0, FONT_HEIGHT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = firstCharacterCode; c <= 126; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                        0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character;
        character.textureId = textureId;
        character.w = face->glyph->bitmap.width;
        character.h = face->glyph->bitmap.rows;
        character.bearingX = face->glyph->bitmap_left;
        character.bearingY = face->glyph->bitmap_top;
        character.advance = face->glyph->advance.x/64;
        character.lineHeight = 20;
        characters.push_back(character);
    }
}

Text::Character Text::getCharacter(char characterCode) {
    std::size_t index = characterCode - firstCharacterCode;

    if (index < 0 || index >= characters.size()) {
        PRINTLN("Character code out of range");
        return characters[0];
    }

    return characters[index];
}

void Text::release() {
    for (std::size_t i = 0; i < characters.size(); i++)
    {
        glDeleteTextures(1, &characters[i].textureId);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}
