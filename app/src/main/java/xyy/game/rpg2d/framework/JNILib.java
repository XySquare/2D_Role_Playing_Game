
package xyy.game.rpg2d.framework;

// Wrapper for native library

public class JNILib {

    // Need to be stated at where any native functions are firstly called
    /*static {
        System.loadLibrary("gl2jni");
    }*/

    public static native void onSurfaceCreated();

    /**
     * @param width  the current view width
     * @param height the current view height
     */
    public static native void onSurfaceChanged(int width, int height);

    public static native void onDrawFrame();

    public static native void onTouch(int pointer, short action, int x, int y);

    public static native void onWebViewClosed();
}
