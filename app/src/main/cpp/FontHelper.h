//
// Created by Xyy on 2019/1/14.
//

#ifndef RPG2D_FONTHELPER_H
#define RPG2D_FONTHELPER_H


#include "SpriteBatcher.h"

class FontHelper {

private:

    static constexpr float CHAR_WIDTH = 17.625f;

public:

    static void drawNumber(SpriteBatcher *spriteBatcher, float x, float y, unsigned int num) {

        const unsigned short FONT_SPRITE_Y = 288;
        if (num == 0)
            spriteBatcher->drawSprite(x - CHAR_WIDTH, y, CHAR_WIDTH, 32,
                                      TextureRegion(0, FONT_SPRITE_Y, CHAR_WIDTH, 32));
        else {
            unsigned char i = 0;
            while (num != 0) {
                spriteBatcher->drawSprite(x - CHAR_WIDTH * i - CHAR_WIDTH, y, CHAR_WIDTH, 32,
                                          TextureRegion(CHAR_WIDTH * (num % 10), FONT_SPRITE_Y,
                                                        CHAR_WIDTH, 32));
                num = num / 10;
                i++;
            }
        }
    }

    static void drawChar(SpriteBatcher *spriteBatcher, float x, float y, char c) {

        const unsigned short textSpriteY = 256;
        if (c > ' ' && c < 127) {
            c -= ' ';
            unsigned char row = static_cast<unsigned char>(c / 16);
            unsigned char col = static_cast<unsigned char>(c % 16);
            spriteBatcher->drawSprite(x, y, CHAR_WIDTH, 32,
                                      TextureRegion(col * CHAR_WIDTH, textSpriteY + row * 32,
                                                    CHAR_WIDTH, 32));
        }
    }

    static void drawText(SpriteBatcher *spriteBatcher, float x, float y, const char *text) {

        unsigned char i = 0;
        char c;
        while ((c = text[i]) != '\0') {
            drawChar(spriteBatcher, x + i * CHAR_WIDTH, y, c);
            i++;
        }
    }
};


#endif //RPG2D_FONTHELPER_H
