//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_EVENTLISTENER_H
#define RPG2D_EVENTLISTENER_H


class Event{

public:
    enum{
        RUNNING,
        TRANSFER,
        ITEM,
        BATTLE
    };
};

class EventListener {

public:

    virtual void onEvent(int what, int prop) = 0;

    virtual ~EventListener() {

    }
};


#endif //RPG2D_EVENTLISTENER_H
