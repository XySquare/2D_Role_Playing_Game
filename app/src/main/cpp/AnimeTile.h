//
// Created by Xyy on 2018/12/22.
//

#ifndef RPG2D_ANIMETILE_H
#define RPG2D_ANIMETILE_H


#include "Tile.h"

class AnimeTile final: public Tile {

private:

    const float frameDuration;

    const int *frames;

public:

    AnimeTile(const float frameDuration, const int frameLength, int *frames)
            : Tile(frameLength), frameDuration(frameDuration), frames(frames) {}


    virtual int getGId(float stateTime) override {

        int frameNumber = (int)(stateTime / frameDuration);
        return frames[frameNumber % gid];
    };

    virtual ~AnimeTile() override {

        delete[] frames;
    }
};


#endif //RPG2D_ANIMETILE_H
