//
// Created by Xyy on 2019/1/14.
//

#ifndef RPG2D_LIST_H
#define RPG2D_LIST_H


#include "MultiTouchHandler.h"
#include "SpriteBatcher.h"
#include "Assets.h"
#include "FontHelper.h"

class ListEventListener {

public:

    virtual void onSelectItem(unsigned int index) = 0;

    virtual ~ListEventListener() {

    }
};

class ListView {

private:

    const unsigned short listX;

    const unsigned short listWidth;

    const unsigned short itemHeight;

    const unsigned short displayHeight;

    unsigned int itemCount;

    unsigned int displayItemCount;

    signed char pointer;

    unsigned int pressedItemIndex;

    int listY;

    int offsetY;

    unsigned int firstDisplayItemIndex;

    int firstDisplayItemY;

    int pervTouchY;

    int speed;

    bool isDragging;

    bool isDragAble;

    const std::vector<std::string> &list;

    //暂存移动前列表纵坐标
    int preListY;

    ListEventListener *const eventListener;

public:

    ListView(const unsigned short listX, const unsigned short listWidth,
             const unsigned short itemHeight, const unsigned char margin,
             const std::vector<std::string> &list, ListEventListener *const eventListener)
            : listX(listX), listWidth(listWidth),
              itemHeight(itemHeight + margin),
              displayHeight(itemHeight), list(list), eventListener(eventListener) {
        enter();
    }

    void enter() {
        itemCount = static_cast<int>(list.size());
        displayItemCount = static_cast<unsigned int>(720 / itemHeight + 2);
        if(displayItemCount > itemCount)
            displayItemCount = itemCount;
        pointer = -1;
        pressedItemIndex = itemCount;
        preListY = listY = firstDisplayItemY = firstDisplayItemIndex = 0;
        isDragging = false;
        isDragAble = itemCount * itemHeight > 720;
        speed = 0;
        if (!isDragAble) {
            pressedItemIndex = itemCount;
            listY = firstDisplayItemY = (720 - itemCount * itemHeight) / 2;
        }
    }

    void update(float deltaTime) {

        preListY = listY;
        /**
         * 处理惯性运动
         */
        if (speed != 0) {
            //列表移动
            listY += speed * deltaTime;
            //减速
            speed *= 1 - 2 * deltaTime;
            //边界处理
            if (listY > 0) {
                listY = 0;
                speed = 0;
                isDragging = false;
            } else if (listY < 720 - itemCount * itemHeight) {
                listY = 720 - itemCount * itemHeight;
                speed = 0;
                isDragging = false;
            }
            //更新列表起始索引编号与起始坐标
            firstDisplayItemIndex = static_cast<unsigned int>(-listY / itemHeight);
            firstDisplayItemY = listY + firstDisplayItemIndex * itemHeight;
        }
    }

    void eventHandaler(Touch &event) {

        if (event.action == TOUCH_DOWN) {
            //只允许单一输入
            if (pointer == -1) {
                if (event.x > listX && event.x < listX + listWidth) {
                    pointer = event.pointer;
                    //获取按下时相对于列表顶端的纵坐标偏移量
                    offsetY = listY - event.y;
                    //保存"按下时的纵坐标"
                    pervTouchY = event.y;
                    //按下时，当列表并未移动，则重置"是否拖拽中"为否
                    //当列表正在移动，则使列表停止
                    if (speed == 0)
                        isDragging = false;
                    else
                        speed = 0;
                    //获取按下时相对于"列表起始索引编号"的"当前正在被按下的索引编号"，
                    //并进行边界处理
                    pressedItemIndex = static_cast<unsigned int>((event.y - listY) / itemHeight);
                    if (isDragging || event.y < listY || pressedItemIndex > itemCount)
                        pressedItemIndex = itemCount;
                }
            }
        } else if (event.action == TOUCH_DRAGGED) {
            // 只有当触摸区域在列表范围，且列表可拖拽（长度小于屏幕高度），
            // 并且，到"按下时的纵坐标"距离大于5时，或已经在拖拽中，才执行拖拽
            if (isDragAble
                && (isDragging || pervTouchY - event.y > 5 || pervTouchY - event.y < -5)
                && event.x > listX && event.x < listX + listWidth && pointer == event.pointer) {
                //设置"是否拖拽中"标志为是
                isDragging = true;
                //更新列表纵坐标
                listY = event.y + offsetY;
                //重置"当前正在被按下的索引编号"
                pressedItemIndex = itemCount;
                //边界处理
                if (listY > 0)
                    listY = 0;
                else if (listY < 720 - itemCount * itemHeight)
                    listY = 720 - itemCount * itemHeight;
                //更新列表起始索引编号与起始坐标
                firstDisplayItemIndex = static_cast<unsigned int>(-listY / itemHeight);
                firstDisplayItemY = listY + firstDisplayItemIndex * itemHeight;
            }
        } else if (event.action == TOUCH_UP) {
            //抬起的指针必须为初始按下的指针
            if (pointer == event.pointer) {
                pointer = -1;
                //当触摸区域在列表范围
                if (event.x > listX && event.x < listX + listWidth) {
                    /* event.y>=listY: 处理无法拖拽时的情况，
                    * 在ITEMHEIGHT<event.y<listY时，
                    * index也会=0
                    */
                    //非拖拽中
                    if (!isDragging && event.y >= listY) {
                        //获取"索引编号"
                        unsigned int index = static_cast<unsigned int>((event.y - listY) / itemHeight);
                        //按下与抬起的索引编号应一致，且不应为边界
                        if (index == pressedItemIndex && index < itemCount) {
                            // Click
                            if(eventListener){
                                eventListener->onSelectItem(index);
                            }
                            //设置新楼层数
                            //mStage.setFloor(index);
                            //开始传送
                            //gameScreen.setState(TRANSITION);
                        }
                    }
                    //如果列表移动速度大于5px/s，则赋予惯性
                    if ((preListY - listY) * 60.f > 0
                        || (preListY - listY) * 60.f < 0)
                        speed = (int) ((listY - preListY) * 60.f);
                    else {
                        speed = 0;
                    }
                }
                //重置"当前正在被按下的索引编号"
                pressedItemIndex = itemCount;
            }
        }
    }

    void present(SpriteBatcher *spriteBatcher) {

        spriteBatcher->beginBatch(Assets::ui);

        char i;
        const int TXT_X = listX + 200;
        for (i = 0; i < displayItemCount; i++) {
            int index = firstDisplayItemIndex + i;
            if (index < itemCount) {
                if (index == pressedItemIndex)
                    spriteBatcher->drawSprite(listX, firstDisplayItemY + i * itemHeight, listWidth,
                                              displayHeight,
                                              TextureRegion((384 + 64), 128,
                                                            64,
                                                            64));
                else
                    spriteBatcher->drawSprite(listX, firstDisplayItemY + i * itemHeight, listWidth,
                                              displayHeight,
                                              TextureRegion(384, 128, 64,
                                                            64));
                FontHelper::drawText(spriteBatcher, TXT_X, firstDisplayItemY + i * itemHeight + 28,
                         list[firstDisplayItemIndex + i].c_str());
            }
        }

        spriteBatcher->endBatch();
    }
};


#endif //RPG2D_LIST_H
