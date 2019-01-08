//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_EVENTLISTENER_H
#define RPG2D_EVENTLISTENER_H


enum Event: unsigned char {
    RUNNING,
    TRANSFER,
    ITEM,
    BATTLE,
    DOOR,
    PAUSE,
    SHOP
};

class EventListener {

public:

    virtual void onEvent(int what, int prop) = 0;

    virtual ~EventListener() {

    }
};


#endif //RPG2D_EVENTLISTENER_H
