package xyy.game.rpg2d.framework.impl;

import android.view.MotionEvent;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

import xyy.game.rpg2d.framework.Input;
import xyy.game.rpg2d.framework.Pool;

/**
 * Created by ${XYY} on ${2016/2/13}.
 */
final class MultiTouchHandler implements View.OnTouchListener {
    private final boolean[] isTouched = new boolean[20];
    private final int[] touchX = new int[20];
    private final int[] touchY = new int[20];
    private final Pool<Input.Touch> touchEventPool;
    private final List<Input.Touch> touchEvents = new ArrayList<>();
    private final List<Input.Touch> touchEventsBuffer = new ArrayList<>();
    private final float scaleX;
    private final float scaleY;

    MultiTouchHandler(View view, float scaleX, float scaleY) {
        Pool.PoolObjectFactory<Input.Touch> factory = new Pool.PoolObjectFactory<Input.Touch>() {
            @Override
            public Input.Touch createObject() {
                return new Input.Touch();
            }
        };
        touchEventPool = new Pool<>(factory, 100);
        view.setOnTouchListener(this);

        this.scaleX = scaleX;
        this.scaleY = scaleY;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        synchronized (this) {
            int action = event.getActionMasked();
            int pointerIndex = event.getActionIndex();
            int pointerId = event.getPointerId(pointerIndex);
            Input.Touch touchEvent;

            switch (action) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    touchEvent = touchEventPool.newObject();
                    touchEvent.type = Input.Touch.TOUCH_DOWN;
                    touchEvent.pointer = pointerId;
                    touchEvent.x = touchX[pointerId] = (int) (event
                            .getX(pointerIndex) * scaleX);
                    touchEvent.y = touchY[pointerId] = (int) (event
                            .getY(pointerIndex) * scaleY);
                    isTouched[pointerId] = true;
                    touchEventsBuffer.add(touchEvent);
                    break;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_CANCEL:
                    touchEvent = touchEventPool.newObject();
                    touchEvent.type = Input.Touch.TOUCH_UP;
                    touchEvent.pointer = pointerId;
                    touchEvent.x = touchX[pointerId] = (int) (event
                            .getX(pointerIndex) * scaleX);
                    touchEvent.y = touchY[pointerId] = (int) (event
                            .getY(pointerIndex) * scaleY);
                    isTouched[pointerId] = false;
                    touchEventsBuffer.add(touchEvent);
                    break;

                case MotionEvent.ACTION_MOVE:
                    int pointerCount = event.getPointerCount();
                    for (int i = 0; i < pointerCount; i++) {
                        pointerIndex = i;
                        pointerId = event.getPointerId(pointerIndex);

                        touchEvent = touchEventPool.newObject();
                        touchEvent.type = Input.Touch.TOUCH_DRAGGED;
                        touchEvent.pointer = pointerId;
                        touchEvent.x = touchX[pointerId] = (int) (event
                                .getX(pointerIndex) * scaleX);
                        touchEvent.y = touchY[pointerId] = (int) (event
                                .getY(pointerIndex) * scaleY);
                        touchEventsBuffer.add(touchEvent);
                    }
                    break;
            }
            return true;
        }
    }

    public boolean isTouchDown(int pointer) {
        synchronized (this) {
            return !(pointer < 0 || pointer >= 20) && isTouched[pointer];
        }
    }

    public int getTouchX(int pointer) {
        synchronized (this) {
            if (pointer < 0 || pointer >= 20)
                return 0;
            else
                return touchX[pointer];
        }
    }

    public int getTouchY(int pointer) {
        synchronized (this) {
            if (pointer < 0 || pointer >= 20)
                return 0;
            else
                return touchY[pointer];
        }
    }

    public List<Input.Touch> getTouchEvents() {
        synchronized (this) {
            int len = touchEvents.size();
            for (int i = 0; i < len; i++)
                touchEventPool.free(touchEvents.get(i));
            touchEvents.clear();
            touchEvents.addAll(touchEventsBuffer);
            touchEventsBuffer.clear();
            return touchEvents;
        }
    }
}
