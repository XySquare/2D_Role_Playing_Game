package xyy.game.rpg2d.framework.impl;

import android.view.MotionEvent;
import android.view.View;

import xyy.game.rpg2d.framework.JNILib;

/**
 * Created by ${XYY} on ${2016/3/5}.
 */
public final class AndroidInput implements View.OnTouchListener {

    private final float scaleX;
    private final float scaleY;

    AndroidInput(float scaleX, float scaleY) {

        this.scaleX = scaleX;
        this.scaleY = scaleY;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        int action = event.getActionMasked();
        int pointerIndex = event.getActionIndex();
        int pointerId = event.getPointerId(pointerIndex);
        int x = (int) (event.getX(pointerIndex) * scaleX);
        int y = (int) (event.getY(pointerIndex) * scaleY);

        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                JNILib.onTouch(pointerId, (short) 0, x, y);
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL:
                JNILib.onTouch(pointerId, (short) 1, x, y);
                break;

            case MotionEvent.ACTION_MOVE:
                JNILib.onTouch(pointerId, (short) 2, x, y);
                break;
        }
        return true;
    }
}
