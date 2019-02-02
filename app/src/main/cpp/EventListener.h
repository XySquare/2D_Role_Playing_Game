//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_EVENTLISTENER_H
#define RPG2D_EVENTLISTENER_H


enum Event: unsigned char {
    RESUME,
    RUNNING,
    TRANSFER,
    ITEM,
    BATTLE,
    DOOR,
    PAUSE,
    SHOP,
    SAVE_MAP,
    LOAD_MAP,
    ESCAPE,
    NOTIFICATION,
    MONSTER_INFO,
    WEBVIEW
};

class EventListener {

public:

    virtual void onReceive(Event what, const void *arg) = 0;

    virtual ~EventListener() {

    }
};


#endif //RPG2D_EVENTLISTENER_H
