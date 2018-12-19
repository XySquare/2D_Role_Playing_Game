package xyy.game.rpg2d.framework.impl;

import android.view.MotionEvent;
import android.view.View;

import java.util.List;

import xyy.game.rpg2d.framework.GL2JNILib;
import xyy.game.rpg2d.framework.Input;

/**
 * Created by ${XYY} on ${2016/3/5}.
 */
public final class AndroidInput implements Input, View.OnTouchListener {
    //private final MultiTouchHandler touchHandler;
    private final float scaleX;
    private final float scaleY;

    AndroidInput(View view, float scaleX, float scaleY) {
        //touchHandler = new MultiTouchHandler(view, scaleX, scaleY);
        view.setOnTouchListener(this);
        this.scaleX = scaleX;
        this.scaleY = scaleY;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        //synchronized (this) {
        int action = event.getActionMasked();
        int pointerIndex = event.getActionIndex();
        int pointerId = event.getPointerId(pointerIndex);
        int x = (int) (event.getX(pointerIndex) * scaleX);
        int y = (int) (event.getY(pointerIndex) * scaleY);

        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                GL2JNILib.onTouch(pointerId, (short) 0, x, y);
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL:
                GL2JNILib.onTouch(pointerId, (short) 1, x, y);
                break;

            case MotionEvent.ACTION_MOVE:
                GL2JNILib.onTouch(pointerId, (short) 2, x, y);
                break;
        }
        return true;
        //}
    }

    @Override
    public boolean isTouchDown(int pointer) {
        return false;
    }

    @Override
    public int getTouchX(int pointer) {
        return 0;
    }

    @Override
    public int getTouchY(int pointer) {
        return 0;
    }

    @Override
    public List<Input.Touch> getTouchEvents() {
        return null;
    }
}
