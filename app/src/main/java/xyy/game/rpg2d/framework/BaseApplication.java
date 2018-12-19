package xyy.game.rpg2d.framework;

import android.app.Application;
import android.os.Looper;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.text.DateFormat;
import java.util.Date;

/**
 * 用于处理未知错误。保存错误报告
 * Created by ${XYY} on ${2016/4/26}.
 */

public class BaseApplication extends Application {
    private Game game;

    public void init(Game activity){
        this.game = activity;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        // 程序崩溃时触发线程
        UnCeHandler catchExcep = new UnCeHandler();
        Thread.setDefaultUncaughtExceptionHandler(catchExcep);
    }

    // 创建服务用于捕获崩溃异常
    private class UnCeHandler implements Thread.UncaughtExceptionHandler {

        @Override
        public void uncaughtException(Thread thread, Throwable ex) {

            handleException(ex);

            try{
                Thread.sleep(1000);
            }catch (InterruptedException ignored){
            }

            // 1秒钟后重启应用
            //Intent intent = new Intent(getApplicationContext(), DungeonRPG.class);
            //PendingIntent restartIntent = PendingIntent.getActivity(
            //getApplicationContext(), 0, intent,
            //PendingIntent.FLAG_CANCEL_CURRENT);
            // 原代码使用Intent.FLAG_ACTIVITY_NEW_TASK， 但IDE出现警告，
            // 两个FLAG值相同，也能实现Activity重启，但具体区别尚不明确
            //AlarmManager mgr = (AlarmManager)getSystemService(Context.ALARM_SERVICE);
            //mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 1000,
            //        restartIntent);

            // 关闭当前应用
            android.os.Process.killProcess(android.os.Process.myPid());
        }

    }

    /**
     * 保存错误信息到文件中
     */
    private void saveCatchInfo2File(Throwable ex) {
        ex.printStackTrace();
        Writer writer = new StringWriter();
        PrintWriter printWriter = new PrintWriter(writer);
        DateFormat formatter = DateFormat.getDateTimeInstance();
        String time = formatter.format(new Date());
        printWriter.write(time);
        printWriter.write('\n');
        ex.printStackTrace(printWriter);
        Throwable cause = ex.getCause();
        while (cause != null) {
            cause.printStackTrace(printWriter);
            cause = cause.getCause();
        }
        printWriter.close();
        String sb = writer.toString();
        BufferedWriter out = null;
        try {
            String fileName = "crash";
            out = new BufferedWriter(new OutputStreamWriter(
                    game.getFileIO().writeFile(fileName)));
            out.write(sb);
            //TODO:文件保存完了之后,在应用下次启动的时候去检查错误日志,发现新的错误日志,就发送给开发者
        } catch (Exception ignored) {
        }finally {
            try {
                if (out != null)
                    out.close();
            } catch (IOException ignored) {
            }
        }
    }

    private void handleException(Throwable ex) {

        // 保存错误日志
        saveCatchInfo2File(ex);

        if(ex instanceof OutOfMemoryError){
            new Thread(){
                @Override
                public void run() {
                    Looper.prepare();
                    Toast.makeText(getApplicationContext(), "内存不足.",
                            Toast.LENGTH_SHORT).show();
                    Looper.loop();
                }
            }.start();
        }
        else{
            //使用Toast来显示异常信息
            new Thread(){
                @Override
                public void run() {
                    Looper.prepare();
                    Toast.makeText(getApplicationContext(), "很抱歉,程序出现未知错误,即将退出.",
                            Toast.LENGTH_SHORT).show();
                    Looper.loop();
                }
            }.start();
        }


    }
}

