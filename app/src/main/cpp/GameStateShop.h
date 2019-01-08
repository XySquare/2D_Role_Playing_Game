//
// Created by Xyy on 2019/1/6.
//

#ifndef RPG2D_GAMESTATESHOP_H
#define RPG2D_GAMESTATESHOP_H


#include "Game.h"
#include "World.h"
#include "SpriteBatcher.h"
#include "GameScreen.h"

class GameStateShop: public Screen {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    EventListener *eventListener;

    signed char pointer = -1;

    char index = 0;

    const float charWidth = 282/16.f;

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

    GameStateShop(Game game, World *world, SpriteBatcher *spriteBatcher, EventListener *eventListener) :
            Screen(game), world(world), spriteBatcher(spriteBatcher), eventListener(eventListener) {}

    void onEvent(int what, int prop) override {

    }

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for(Touch e : touchEvents) {
            if (e.action == Action ::TOUCH_DOWN) {
                if(pointer ==-1){
                    pointer = e.pointer;
                    if(e.x > 1216 && e.y < 64)
                        index = 1;
                    else if(inBound(e,64,64,544,256))
                        index = 2;
                    else if(inBound(e,672,64,544,256))
                        index = 3;
                    else if(inBound(e,64,384,544,256))
                        index = 4;
                    else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if(pointer == e.pointer) {
                    pointer = -1;
                    if(e.x > 1216 && e.y < 64 && index == 1)
                        eventListener->onEvent(Event::RUNNING, 0);
                    else if(inBound(e,64,64,544,256) &&  index == 2)
                        ;//mStage.buyPoison();
                    else if(inBound(e,672,64,544,256) && index == 3)
                        ;//mStage.buyMaxHpStone();
                    else if(inBound(e,64,384,544,256) && index == 4)
                        ;//mStage.buyAtkStone();
                    break;
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
                                  TextureRegion(448/512.f, 64/512.f, 64/512.f, 64/512.f));

        drawText(1280/2-charWidth*2,64+32,"SHOP");

        spriteBatcher->drawSprite(64+64,64+96,544-64,200,
                                  TextureRegion(320/512.f, 352/512.f, 192/512.f, 32/512.f));

        drawText(64+64+16+64+16,64+96+16,"ATK +2");

        drawText(64+64+16+64+16,64+96+16+32+16,"25 G");

        spriteBatcher->drawSprite(672,64+96,544-64,200,
                                  TextureRegion(320/512.f, 352/512.f, 192/512.f, 32/512.f));

        spriteBatcher->drawSprite(64+64,64+96+200+64,544-64,200,
                                  TextureRegion(320/512.f, 352/512.f, 192/512.f, 32/512.f));

        spriteBatcher->endBatch();
    }
};


#endif //RPG2D_GAMESTATESHOP_H
