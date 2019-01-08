//
// Created by Xyy on 2018/12/18.
//

#ifndef RPG2D_MULTITOUCHHANDLER_H
#define RPG2D_MULTITOUCHHANDLER_H


#include <mutex>
#include <vector>

enum Action : unsigned char {
    TOUCH_DOWN = 0,
    TOUCH_UP,
    TOUCH_DRAGGED
};

struct Touch{

    unsigned char pointer;
    Action action;
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

    /**
     * Remember to call this function to clear the buffer even if no inputs are required
     */
    std::vector<Touch>& getTouchEvents(){

        touchEvents.clear();
        syn.lock();
        touchEvents.insert(touchEvents.end(), touchEventsBuffer.begin(), touchEventsBuffer.end());
        touchEventsBuffer.clear();
        syn.unlock();
        return touchEvents;
    }
};


#endif //RPG2D_MULTITOUCHHANDLER_H
