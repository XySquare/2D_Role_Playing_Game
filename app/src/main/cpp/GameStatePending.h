/**
 * One of the states of GameScreen. Actives typically when WebView is shown.
 * Shows nothing on it.
 *
 * Created by Xyy on 2019/9/1.
 */
#ifndef RPG2D_GAMESTATEPENDING_H
#define RPG2D_GAMESTATEPENDING_H


#include "Screen.h"

class GameStatePending final : public Screen  {

public:
    GameStatePending(const Game &game) : Screen(game) {}

    void onReceive(Event what, const void *arg) override {

    }

    void resume() override {

    }

    void update(float deltaTime) override {

    }

    void present() override {

    }
};


#endif //RPG2D_GAMESTATEPENDING_H
