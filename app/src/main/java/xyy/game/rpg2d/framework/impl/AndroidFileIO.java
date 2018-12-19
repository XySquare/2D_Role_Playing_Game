package xyy.game.rpg2d.framework.impl;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import xyy.game.rpg2d.framework.FileIO;

/**
 * Game接口的实现将保存该类的一个实例
 * 通过传入Context
 * Created by ${XYY} on ${2015/3/5}.
 */
public final class AndroidFileIO implements FileIO {
    private final AssetManager mAssets;
    //外部储存路径
    private String mExternalStoragePath;

    AndroidFileIO(Context context, AssetManager assets, String gameId) {
        this.mAssets = assets;
        //this.externalStoragePath = Environment.getExternalStorageDirectory()
        //       .getAbsolutePath() + File.separator + "dungeonRPG" + File.separator;
        File externalFilesDir = context.getExternalFilesDir(null);
        if (externalFilesDir != null) {
            mExternalStoragePath = externalFilesDir.toString() + File.separatorChar + "GameDate" + File.separatorChar + gameId + File.separatorChar;
            File destDir = new File(mExternalStoragePath);
            if (!destDir.exists()) {
                if (!destDir.mkdirs())
                    Log.e("FileIO", "Failed to create directories: " + mExternalStoragePath);
            }
        } else {
            Log.e("FileIO", "External storage unavailable.");
        }
    }

    @Override
    public String getExternalStoragePath() {
        return mExternalStoragePath;
    }

    /*@Override
    public InputStream readAsset(String fileName) throws IOException {
        return mAssets.open(fileName);
    }*/

    @Override
    public InputStream readFile(String fileName) throws IOException {
        return new FileInputStream(mExternalStoragePath + fileName);
    }

    @Override
    public OutputStream writeFile(String fileName) throws IOException {
        File file = new File(mExternalStoragePath + fileName);
        File parent = file.getParentFile();
        if (!parent.exists())
            if (!parent.mkdirs())
                Log.e("FileIO", "Failed to create directories: " + parent.toString());
        return new FileOutputStream(file);
    }

    /*@Override
    public OutputStream appendFile(String fileName) throws IOException {
        return new FileOutputStream(mExternalStoragePath + fileName, true);
    }*/

    @Override
    public boolean deleteFile(String fileName) {
        File file = new File(mExternalStoragePath + fileName);
        return file.exists() && file.delete();
    }

    @Override
    public boolean exists(String fileName) {
        File file = new File(mExternalStoragePath + fileName);
        return file.exists();
    }

    @Override
    public File[] listFIle(String pathName) {
        File destDir = new File(mExternalStoragePath + (pathName == null ? "" : pathName));
        return destDir.listFiles();
    }
}
