package xyy.game.rpg2d.framework.impl;

import android.media.MediaPlayer;

import java.io.FileInputStream;
import java.io.IOException;

import xyy.game.rpg2d.framework.Music;

/**
 * Created by ${XYY} on ${2015/3/5}.
 */
public final class AndroidMusic implements Music, MediaPlayer.OnCompletionListener {
    private static float volume = 1;
    private final MediaPlayer mediaPlayer;
    //跟踪mediaPlayer准备状态，只有准备完成后，才能调用mediaPlayer.start()/stop()/pause()
    private boolean isPrepared = false;

    /**
     * 通过传入的AssetFileDescriptor创建和准备MediaPlayer
     * //@param assetDescriptor assetDescriptor实例
     */
    /*AndroidMusic(AssetFileDescriptor assetDescriptor) {
        mediaPlayer = new MediaPlayer();
        try {
            mediaPlayer.setDataSource(assetDescriptor.getFileDescriptor(),
                    assetDescriptor.getStartOffset(),
                    assetDescriptor.getLength());
            //准备播放
            mediaPlayer.prepare();
            isPrepared = true;
            mediaPlayer.setOnCompletionListener(this);
            //默认为循环播放
            mediaPlayer.setLooping(true);
            //音量同步其他音乐
            mediaPlayer.setVolume(volume, volume);
        } catch (Exception e) {
            throw new RuntimeException("Couldn't load music");
        }
    }*/

    AndroidMusic(FileInputStream fileInputStream) {
        mediaPlayer = new MediaPlayer();
        try {
            mediaPlayer.setDataSource(fileInputStream.getFD());
            //准备播放
            mediaPlayer.prepare();
            isPrepared = true;
            mediaPlayer.setOnCompletionListener(this);
            //默认为循环播放
            mediaPlayer.setLooping(true);
            //音量同步其他音乐
            mediaPlayer.setVolume(volume, volume);
        } catch (Exception e) {
            throw new RuntimeException("Couldn't load music");
        }
    }

    @Override
    public void play() {
        //如果正在播放，则直接返回
        if (mediaPlayer.isPlaying())
            return;

        try {
            //由于使用了isPrepared标志，
            //该标志是在一个单独的线程中设置（实现了OnCompletionListener接口）
            //所以工作在一个同步块中进行
            synchronized (this) {
                //检查mediaPlayer是否准备好，如果没有则准备
                if (!isPrepared)
                    mediaPlayer.prepare();
                mediaPlayer.start();
            }
        } catch (IllegalStateException | IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void stop() {
        mediaPlayer.stop();
        //在同步块中设置isPrepared标志
        synchronized (this) {
            isPrepared = false;
        }
    }

    @Override
    public void pause() {
        //检查是否在播放，如果是，则暂停
        if (mediaPlayer.isPlaying())
            mediaPlayer.pause();
    }

    @Override
    public void setLooping(boolean looping) {
        mediaPlayer.setLooping(looping);
    }

    @Override
    public void setVolume(float volume) {
        mediaPlayer.setVolume(volume, volume);
        AndroidMusic.volume = volume;
    }

    @Override
    public boolean isPlaying() {
        return mediaPlayer.isPlaying();
    }

    @Override
    public boolean isStopped() {
        return !isPrepared;
    }

    @Override
    public boolean isLooping() {
        return mediaPlayer.isLooping();
    }

    @Override
    public void dispose() {
        //先检查音乐是否在播放，如果是，则先停止，再释放
        //直接释放将抛出RuntimeException
        if (mediaPlayer.isPlaying())
            mediaPlayer.stop();
        mediaPlayer.reset();
        mediaPlayer.release();
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        //在同步块中设置isPrepared标志
        synchronized (this) {
            isPrepared = false;
        }
    }
}
