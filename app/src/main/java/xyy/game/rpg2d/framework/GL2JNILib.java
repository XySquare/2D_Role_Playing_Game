
package xyy.game.rpg2d.framework;

// Wrapper for native library

public class GL2JNILib {

    // Used to load the native library on application startup.
    static {
        System.loadLibrary("gl2jni");
    }

    public static native void onSurfaceCreated();

    /**
     * @param width  the current view width
     * @param height the current view height
     */
    public static native void onSurfaceChanged(int width, int height);

    public static native void onDrawFrame();

    public static native void onTouch(int pointer, short action, int x, int y);
}
