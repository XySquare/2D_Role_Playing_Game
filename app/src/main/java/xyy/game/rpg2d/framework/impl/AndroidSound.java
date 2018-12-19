package xyy.game.rpg2d.framework.impl;

import android.media.SoundPool;


import xyy.game.rpg2d.framework.Sound;


/**
 * Created by ${XYY} on ${2016/3/5}.
 */
public final class AndroidSound implements Sound {
    private static float volume = 1;
    private final int soundId;
    private final SoundPool soundPool;

    /**
     * 储存soundPool和该音效的ID，以便后续播放和释放
     * @param soundPool soundPool实例
     * @param soundId 该音效的ID
     */
    AndroidSound(SoundPool soundPool, int soundId) {
        this.soundId = soundId;
        this.soundPool = soundPool;
    }

    @Override
    public void play() {
        soundPool.play(soundId, volume, volume, 0, 0, 1);
    }

    @Override
    public void dispose() {
        soundPool.unload(soundId);
    }

    public static void setVolume(float volume){
        AndroidSound.volume = volume;
    }
}
