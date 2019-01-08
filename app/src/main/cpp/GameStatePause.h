//
// Created by Xyy on 2018/12/30.
//

#ifndef RPG2D_GAMESTATEPAUSE_H
#define RPG2D_GAMESTATEPAUSE_H


#include "Screen.h"
#include "TextureRegion.h"
#include "World.h"
#include "SpriteBatcher.h"
#include "SaveDataHelper.h"

class GameStatePause final: public Screen {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    EventListener *eventListener;

    const float charWidth = 282/16.f;

    void drawNumber(float x, float y, unsigned int num) {

        const unsigned short textSpriteY = 256 + 32;
        if (num == 0)
            spriteBatcher->drawSprite(x - charWidth, y, charWidth, 32,
                                      TextureRegion(0, textSpriteY / 512.f,
                                                    charWidth / 512.f, 32 / 512.f));
        else {
            unsigned char i = 0;
            while (num != 0) {
                spriteBatcher->drawSprite(x - charWidth * i - charWidth, y, charWidth, 32,
                                          TextureRegion(charWidth / 512.f * (num % 10), textSpriteY / 512.f,
                                                        charWidth / 512.f, 32 / 512.f));
                num = num / 10;
                i++;
            }
        }

    }

    void drawChar(float x, float y, char c) {

        const unsigned short textSpriteY = 256;
        if(c > ' ' && c < 127){
            c -= ' ';
            unsigned char row = static_cast<unsigned char>(c / 16);
            unsigned char col = static_cast<unsigned char>(c % 16);
            spriteBatcher->drawSprite(x, y, charWidth, 32, TextureRegion(col*charWidth / 512.f, (textSpriteY + row*32) / 512.f, charWidth / 512.f, 32 / 512.f));
        }
    }

    void drawText(float x, float y, const char* text){

        unsigned char i = 0;
        char c;
        while((c = text[i]) != '\0'){
            drawChar(x + i*charWidth, y, c);
            i++;
        }
    }

    bool inBound(Touch e, short x, short y, unsigned short width, unsigned short height){
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:

    GameStatePause(Game game, World *world, SpriteBatcher *spriteBatcher, EventListener *eventListener) :
            Screen(game), world(world), spriteBatcher(spriteBatcher), eventListener(eventListener) {}

    void onEvent(int what, int prop) override {

    }

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (e.action == Action::TOUCH_UP) {
                if (e.x > 1280 - 80 && e.y < 80) {
                    eventListener->onEvent(Event::RUNNING, 0);
                }
                else if(inBound(e,1280-64-192-192-18, 512+64, 192, 96)){
                    // Save
                    SaveDataHelper::save(game.fileIO, world);
                }
                else if(inBound(e,1280-64-192, 512+64, 192, 96)){
                    // Load
                    SaveDataHelper::load(game.fileIO, world);
                }
            }
        }
    }

    void present() override {

        // Background
        spriteBatcher->beginBatch(Assets::unitTexture);
        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL, 0.f, 0.f, 0.f, .5f);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        spriteBatcher->beginBatch(Assets::ui);

        // Close
        spriteBatcher->drawSprite(1280-64, 0, 64, 64,
                                  TextureRegion(448/512.f, 64/512.f, (float) 64 / 512, (float) 64 / 512));

        drawText(910, 64, "LV");
        drawNumber(1018, 64, 999);

        drawText(1036, 64, "NEXT");
        drawNumber(1216, 64, world->exp);

        drawText(964, 128, "HP");
        drawNumber(1108, 128, world->player.prop->hp);
        drawText(1108, 128, "/");
        drawNumber(1216, 128, world->player.prop->maxHp);

        drawText(1090, 192, "ATK");
        drawNumber(1216, 192, world->player.prop->atk);
        drawText(1090, 256, "DEF");
        drawNumber(1216, 256, world->player.prop->def);
        drawText(1090, 320, "AGI");
        drawNumber(1216, 320, world->player.prop->agi);

        // Coin
        spriteBatcher->drawSprite(1072-90, 384, 32, 32,
                                  TextureRegion(32/512.f, (448+32)/512.f, (float) 32 / 512, (float) 32 / 512));
        drawNumber(1216, 384, world->coin);

        spriteBatcher->drawSprite(1072-90, 192, 32, 32,
                                  TextureRegion(0/512.f, 448/512.f, (float) 32 / 512, (float) 32 / 512));
        drawNumber(1072, 192, world->bag->key[0]);
        spriteBatcher->drawSprite(1072-90, 256, 32, 32,
                                  TextureRegion(32/512.f, 448/512.f, (float) 32 / 512, (float) 32 / 512));
        drawNumber(1072, 256, world->bag->key[1]);
        spriteBatcher->drawSprite(1072-90, 320, 32, 32,
                                  TextureRegion(0/512.f, (448+32)/512.f, (float) 32 / 512, (float) 32 / 512));
        drawNumber(1072, 320, world->bag->key[2]);

        // Potion
        spriteBatcher->drawSprite(1280-32-128, 448, 96, 96,
                                  TextureRegion(416/512.f, 128/512.f, (float) 96 / 512, (float) 96 / 512));
        drawNumber(1280-32-128+96, 448+96-32, world->bag->potion);

        // LOAD
        spriteBatcher->drawSprite(1280-64-192, 512+64, 192, 96,
                                  TextureRegion(320/512.f, 256/512.f, (float) 192 / 512, (float) 96 / 512));

        // SAVE
        spriteBatcher->drawSprite(1280-64-192-192-18, 512+64, 192, 96,
                                  TextureRegion(320/512.f, (256+128)/512.f, (float) 192 / 512, (float) 96 / 512));

        spriteBatcher->endBatch();
    }

    ~GameStatePause() override {

    }
};


#endif //RPG2D_GAMESTATEPAUSE_H
