/**
 * Manage the notifications displayed at the top left of the screen for 2.7s.
 *
 * Created by Xyy on 2019/1/11.
 */

#ifndef RPG2D_NOTIFICATION_H
#define RPG2D_NOTIFICATION_H


#include "SpriteBatcher.h"

class Notification {

private:

    // Specify the maximum message queen length
    static const unsigned char MAX_MESSAGE_NUM = 3;

    float timer;

    std::string *msg;

    int head, tail;

    int x;

    const float charWidth = 282 / 16.f;

    void drawChar(SpriteBatcher *spriteBatcher, float x, float y, char c) {

        const unsigned short textSpriteY = 256;
        if (c > ' ' && c < 127) {
            c -= ' ';
            unsigned char row = static_cast<unsigned char>(c / 16);
            unsigned char col = static_cast<unsigned char>(c % 16);
            spriteBatcher->drawSprite(x, y, charWidth, 32, TextureRegion(col * charWidth / 512.f,
                                                                         (textSpriteY + row * 32) /
                                                                         512.f, charWidth / 512.f,
                                                                         32 / 512.f));
        }
    }

    void drawText(SpriteBatcher *spriteBatcher, float x, float y, const char *text) {

        unsigned char i = 0;
        char c;
        while ((c = text[i]) != '\0') {
            drawChar(spriteBatcher, x + i * charWidth, y, c);
            i++;
        }
    }

public:

    Notification() : timer(0), msg(new std::string[MAX_MESSAGE_NUM]), head(0), tail(0), x(-384) {}

    void update(float deltaTime) {

        if (head != tail) {
            timer += deltaTime;
            if (timer < 0.2) {
                x = (int) (384 * (5 * timer - 1));
            } else if (timer < 2.5) {
                x = 0;
            } else if (timer < 2.7) {
                x = (int) (384 * (5 * (2.7 - timer) - 1));
            } else {
                x = -384;
                timer = 0;
                head = (head + 1) % MAX_MESSAGE_NUM;
            }
        }
    }

    void present(SpriteBatcher *spriteBatcher) {

        if (head != tail) {
            spriteBatcher->beginBatch(Assets::ui);
            spriteBatcher->drawSprite(x, 64, 384, 96,
                                      TextureRegion(384 / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
            drawText(spriteBatcher, x + 32, 64 + 32, msg[(head + 1) % MAX_MESSAGE_NUM].c_str());
            spriteBatcher->endBatch();
        }
    }

    void newNotice(const char *m) {

        tail = (tail + 1) % MAX_MESSAGE_NUM;
        if (tail == head) {
            LOGI("Notification", "Too many notifications, discard the oldest one.");
            head = (head + 1) % MAX_MESSAGE_NUM;
        }
        msg[tail] = m;
    }

    virtual ~Notification() {

        delete[]msg;
    }
};


#endif //RPG2D_NOTIFICATION_H
