//
// Created by Xyy on 2018/12/18.
//

#ifndef RPG2D_MULTITOUCHHANDLER_H
#define RPG2D_MULTITOUCHHANDLER_H


#include <mutex>
#include <vector>

const static short TOUCH_DOWN = 0;

const static short TOUCH_UP = 1;

const static short TOUCH_DRAGGED = 2;

struct Touch{

    short pointer;
    short action;
    short x;
    short y;
};

class MultiTouchHandler {

private:

    std::mutex syn;
    std::vector<Touch> touchEvents;
    std::vector<Touch> touchEventsBuffer;

public:

    void onTouch(Touch event){

        syn.lock();
        touchEventsBuffer.push_back(event);
        syn.unlock();
    }

    std::vector<Touch> getTouchEvents(){

        touchEvents.clear();
        syn.lock();
        touchEvents.insert(touchEvents.end(), touchEventsBuffer.begin(), touchEventsBuffer.end());
        touchEventsBuffer.clear();
        syn.unlock();
        return touchEvents;
    }
};


#endif //RPG2D_MULTITOUCHHANDLER_H
