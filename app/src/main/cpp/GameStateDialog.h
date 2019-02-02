/**
 * One of the states of GameScreen. Active when an operation need to be approved by player.
 * For example, when "ESC" is pressed on pause menu.
 *
 * Created by Xyy on 2019/1/17.
 */

#ifndef RPG2D_GAMESTATEDIALOG_H
#define RPG2D_GAMESTATEDIALOG_H


#include "Screen.h"
#include "FontHelper.h"

enum Result: unsigned char {

    APPROVE,
    CANCEL
};

class DialogListener {

public:

    virtual void onDialogResult(unsigned char dialogId, Result res) = 0;

    virtual ~DialogListener() {

    }
};

class GameStateDialog final : public Screen {

private:

    const unsigned char dialogId;

    const std::string msgText;

    const std::string comfirmText;

    const std::string cancelText;

    DialogListener *const dialogListener;

    signed char pointer = -1;

    unsigned char index = 0;

    bool inBound(Touch e, short x, short y, unsigned short width, unsigned short height) {
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:

    GameStateDialog(const Game &game, const unsigned char dialogId, const std::string &msgText,
                        const std::string &comfirmText,
                        const std::string &cancelText, DialogListener *const cancelListener)
            : Screen(game), dialogId(dialogId), msgText(msgText),
              comfirmText(comfirmText), cancelText(cancelText),
              dialogListener(cancelListener) {}

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (inBound(e, 814, 576, 192, 96)) {
                        index = 2;
                    } else if (inBound(e, 1024, 576, 192, 96)) {
                        index = 3;
                    } else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (inBound(e, 814, 576, 192, 96) && index == 2) {
                        // Confirm
                        if(dialogListener)
                            dialogListener->onDialogResult(dialogId, APPROVE);
                    } else if (inBound(e, 1024, 576, 192, 96) && index == 3) {
                        // Cancel
                        if(dialogListener)
                            dialogListener->onDialogResult(dialogId, CANCEL);
                    }
                    index = 0;
                    break;
                }
            }
        }
    }

    void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        // Background
        spriteBatcher->beginBatch(0.f, 0.f, 0.f, .5f);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        spriteBatcher->beginBatch(Assets::ui);

        FontHelper::drawText(spriteBatcher, 128, 128, msgText.c_str());

        // Confirm
        if(!comfirmText.empty()){
            spriteBatcher->drawSprite(814, 576, 192, 96,
                                      TextureRegion((index == 2 ? 448 : 384), 128, 64, 64));
            FontHelper::drawText(spriteBatcher, 862, 612, comfirmText.c_str());
        }

        // Cancel
        spriteBatcher->drawSprite(1024, 576, 192, 96,
                                  TextureRegion((index == 3 ? 448 : 384), 128, 64, 64));
        FontHelper::drawText(spriteBatcher, 1072, 612, cancelText.c_str());

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }

    virtual ~GameStateDialog() {

    }

};


#endif //RPG2D_GAMESTATEDIALOG_H
