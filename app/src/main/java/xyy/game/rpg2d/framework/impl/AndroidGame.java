package xyy.game.rpg2d.framework.impl;

import android.content.res.Configuration;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;

import xyy.game.rpg2d.framework.Audio;
import xyy.game.rpg2d.framework.FileIO;
import xyy.game.rpg2d.framework.Game;
import xyy.game.rpg2d.framework.Input;

/**
 * AndroidGame抽象类
 * 当前标准分辨率为1280*720
 * 需实现getStartScreen()方法以开始游戏
 * Created by ${XYY} on ${2016/3/5}.
 */
public abstract class AndroidGame extends AppCompatActivity implements Game {

    protected static final String EXTRA_GAME_ID = "com.xyy.game.dungeonrpg.game_id";

    private static final String DIALOG_SINGLE = "dialog_single";

    private AndroidFastRenderView renderView;
    //private Graphics graphics;
    private Audio audio;
    private Input input;
    private FileIO fileIO;
    //private Screen screen;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.w("AndroidGame","onCreate");
        String gameId = getIntent().getStringExtra(EXTRA_GAME_ID);

        //初始化未捕获异常处理
        //BaseApplication application = (BaseApplication) getApplication();
        //application.init(this);

        //根据设备方向，定义帧缓冲区大小，实例化Bitmap
        boolean isLandscape = getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE;
        int frameBufferWidth = isLandscape ? 1280 : 720;
        int frameBufferHeight = isLandscape ? 720 : 1280;
        /*Bitmap frameBuffer = Bitmap.createBitmap(frameBufferWidth,
                frameBufferHeight, Bitmap.Config.ARGB_8888);*/

        //获取屏幕宽/高
        Point outSize = new Point();
        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            //getWindowManager().getDefaultDisplay().getRealSize(outSize);
        //} else
            getWindowManager().getDefaultDisplay().getSize(outSize);
        //Log.i("AndroidGame", "Size = " + outSize);
        //计算缩放比例
        float scaleX = (float) frameBufferWidth
                / outSize.x;
        float scaleY = (float) frameBufferHeight
                / outSize.y;

        renderView = new AndroidFastRenderView(this);
        //graphics = new AndroidGraphics(getAssets(), frameBuffer);
        fileIO = new AndroidFileIO(this, getAssets(), gameId);
        audio = null;//new AndroidAudio(this, fileIO);
        input = new AndroidInput(renderView, scaleX, scaleY);

        setContentView(renderView);
    }

    /*
    * 返回键事件处理
    * */
    @Override
    public boolean onKeyDown(int keyCode, @NonNull KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            /*if (screen.onBackPressed()) */return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.w("AndroidGame","onResume");
        /*设置重力感应横屏 */
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        /* Window flag: as long as this window is visible to the user, keep
           the device's screen turned on and bright. */
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        /*使活动全屏，该Flag可能被OS清除，因此需再次设置 */
        int visibility = View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                View.SYSTEM_UI_FLAG_FULLSCREEN;
        /*Android 4.4, API 19
          沉浸式全屏*/
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            visibility |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        }
        getWindow().getDecorView().setSystemUiVisibility(visibility);
        //screen.resume();
        renderView.onResume();
        //Log.i("Activity", "Resume");
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.w("AndroidGame","onPause");
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        /*if (isFinishing())
            renderView.dispose();
        else*/
            renderView.onPause();
        //screen.pause();

        //如果活动将被销毁，则释放screen
        //if (isFinishing())
        //screen.dispose();
    }

    @Override
    public Input getInput() {
        return input;
    }

    @Override
    public FileIO getFileIO() {
        return fileIO;
    }

    @Override
    public Audio getAudio() {
        return audio;
    }

    public AndroidFastRenderView getRenderView() {
        return renderView;
    }

    /*public Screen getCurrentScreen() {
        return screen;
    }*/

    @Override
    public void showDialog(DialogFragment dialogFragment) {
        FragmentManager manager = getSupportFragmentManager();
        FragmentTransaction ft = manager.beginTransaction();
        ft.add(dialogFragment, DIALOG_SINGLE);
        ft.commit();
    }

}
