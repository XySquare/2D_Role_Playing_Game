/**
 * One of the states of GameScreen. Active when player hold a monster object on the map.
 * Screen will show the detail information of the monster.
 * Will back to Running state when player release its figure.
 *
 * Created by Xyy on 2019/1/13.
 */

#ifndef RPG2D_GAMESTATEMONSTERINFO_H
#define RPG2D_GAMESTATEMONSTERINFO_H


#include "Screen.h"
#include "World.h"

class GameStateMonsterInfo final : public Screen {

private:

    EventListener *eventListener;

    Monster *monster;

    bool isRight;

public:
    GameStateMonsterInfo(const Game &game, World *world,
                         EventListener *eventListener, unsigned int monserId, bool isRight) :
            Screen(game),
            eventListener(eventListener), isRight(isRight) {

        monster = world->db->getMonsterById(monserId);
    }

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (e.action == Action::TOUCH_UP) {
                eventListener->onReceive(RUNNING, NULL);
            }
        }
    }

    void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        // Background
        spriteBatcher->beginBatch(0.f, 0.f, 0.f, .5f);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        unsigned short x = static_cast<unsigned short>(isRight ? 910 : 64);

        spriteBatcher->beginBatch(Assets::ui);

        FontHelper::drawText(spriteBatcher, x, 64, "LV");
        FontHelper::drawNumber(spriteBatcher, x + 306, 64, monster->prop->lv);

        FontHelper::drawText(spriteBatcher, x, 128, "HP");
        FontHelper::drawNumber(spriteBatcher, x + 306, 128, monster->prop->maxHp);

        FontHelper::drawText(spriteBatcher, x, 192, "ATK");
        FontHelper::drawNumber(spriteBatcher, x + 306, 192, monster->prop->atk);
        FontHelper::drawText(spriteBatcher, x, 256, "DEF");
        FontHelper::drawNumber(spriteBatcher, x + 306, 256, monster->prop->def);
        FontHelper::drawText(spriteBatcher, x, 320, "AGI");
        FontHelper::drawNumber(spriteBatcher, x + 306, 320, monster->prop->agi);

        // Coin
        spriteBatcher->drawSprite(x, 376, 48, 48,
                                  TextureRegion(0, (448), 64,
                                                64));
        FontHelper::drawNumber(spriteBatcher, x + 306, 384, monster->coin);

        FontHelper::drawText(spriteBatcher, x, 448, "EXP");
        FontHelper::drawNumber(spriteBatcher, x + 306, 448, monster->exp);

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }

    virtual ~GameStateMonsterInfo() {

    }
};


#endif //RPG2D_GAMESTATEMONSTERINFO_H
