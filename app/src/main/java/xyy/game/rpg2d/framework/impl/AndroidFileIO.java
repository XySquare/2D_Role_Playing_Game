package xyy.game.rpg2d.framework.impl;

import android.content.Context;
import android.util.Log;

import java.io.File;

import xyy.game.rpg2d.framework.GL2JNILib;

/**
 * Game接口的实现将保存该类的一个实例
 * 通过传入Context
 * Created by ${XYY} on ${2015/3/5}.
 */
final class AndroidFileIO {

    AndroidFileIO(Context context) {
        //this.externalStoragePath = Environment.getExternalStorageDirectory()
        //       .getAbsolutePath() + File.separator + "dungeonRPG" + File.separator;
        File externalFilesDir = context.getExternalFilesDir(null);
        if (externalFilesDir != null) {
            //外部储存路径
            String externalStoragePath = externalFilesDir.toString();
            if(!externalStoragePath.endsWith(File.separator))
                externalStoragePath += File.separatorChar;
            GL2JNILib.setupFileIO(externalStoragePath, context.getAssets());
        } else {
            Log.e("FileIO", "External storage unavailable.");
        }
    }
}
