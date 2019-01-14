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
    SHOP,
    LOAD_MAP,
    ESCAPE,
    NOTIFICATION,
    MONSTER_INFO
};

class EventListener {

public:

    virtual void onReceive(Event what, const void *arg) = 0;

    virtual ~EventListener() {

    }
};


#endif //RPG2D_EVENTLISTENER_H
