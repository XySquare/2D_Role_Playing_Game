package xyy.game.rpg2d.framework.impl;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Build;
import android.support.annotation.Nullable;

import java.io.FileInputStream;
import java.io.IOException;

import xyy.game.rpg2d.framework.*;

/**
 * Created by ${XYY} on ${2016/3/5}.
 */
public final class AndroidAudio implements Audio {
    private final AssetManager assets;
    private final SoundPool soundPool;
    private final FileIO mFileIO;

    AndroidAudio(Activity activity, FileIO fileIO) {
        //设置媒体流的音量控制
        activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
        this.assets = activity.getAssets();
        this.mFileIO = fileIO;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            //This constructor was deprecated in API level 21.
            //use SoundPool.Builder instead to create and configure a SoundPool instance
            soundPool = new SoundPool(20, AudioManager.STREAM_MUSIC, 0);
        }else{
            SoundPool.Builder soundPoolBuilder = new SoundPool.Builder();
            soundPoolBuilder.setMaxStreams(20);
            soundPoolBuilder.setAudioAttributes(new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_GAME).build());
            soundPool = soundPoolBuilder.build();
        }
    }

    @Override
    @Nullable
    public Music newMusic(String filename) {
        try {
            //AssetFileDescriptor assetDescriptor = assets.openFd(filename);
            return new AndroidMusic((FileInputStream) mFileIO.readFile(filename));
        } catch (IOException e) {
            e.printStackTrace();
            //该错误无法恢复，因此以RuntimeException代替IOException抛出
            //Log.e("AndroidAudio","Couldn't load music '" + filename + "'");
            return null;
        }
    }

    @Override
    public Sound newSound(String filename) {
        try {
            AssetFileDescriptor assetDescriptor = assets.openFd(filename);
            int soundId = soundPool.load(assetDescriptor, 1);
            return new AndroidSound(soundPool, soundId);
        } catch (IOException e) {
            //该错误无法恢复，因此以RuntimeException代替IOException抛出
            throw new RuntimeException("Couldn't load sound '" + filename + "'");
        }
    }

}
