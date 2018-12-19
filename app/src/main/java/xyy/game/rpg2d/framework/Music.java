package xyy.game.rpg2d.framework;

/**
 * Music接口
 * Created by ${XYY} on ${2015/11/20}.
 */
public interface Music {
    /**
     * 播放音乐
     */
    void play();

    /**
     * 停止音乐，将从准备状态中退出
     */
    void stop();

    /**
     * 暂停音乐
     */
    void pause();

    /**
     * 设置是否循环
     * @param looping true: 循环
     */
    void setLooping(boolean looping);

    /**
     * 设置音量
     * @param volume 音量（0~1），左右声道相同
     */
    void setVolume(float volume);

    /**
     * 返回音乐是否播放
     * @return true: 音乐正在播放
     * false: 暂停/停止
     */
    boolean isPlaying();

    /**
     * 返回音乐是否停止
     * @return true:停止
     * false: 播放/暂停
     */
    boolean isStopped();

    /**
     * 返回音乐是否循环
     * @return true: 音乐循环播放
     */
    boolean isLooping();

    /**
     * 释放该音乐
     */
    void dispose();
}
