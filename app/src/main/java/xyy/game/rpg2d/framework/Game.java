package xyy.game.rpg2d.framework;

import android.opengl.GLSurfaceView;
import android.support.v4.app.DialogFragment;

/**
 * Game接口
 * Created by ${XYY} on ${2015/11/20}.
 */
public interface Game {
    Input getInput();

    FileIO getFileIO();

    //Graphics getGraphics();

    Audio getAudio();

    //void setScreen(Screen screen);

    //GLSurfaceView.Renderer getRenderer();

    void showDialog(DialogFragment dialogFragment);
}
