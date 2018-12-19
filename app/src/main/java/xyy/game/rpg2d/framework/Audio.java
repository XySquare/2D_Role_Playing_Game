package xyy.game.rpg2d.framework;

import android.support.annotation.Nullable;

/**
 * Audio接口
 * 负责从资源文件中创建声音和音乐实例
 * Created by ${XYY} on ${2015/11/20}.
 */
public interface Audio {
    /**
     * 从Assets文件夹中加载音频，音频将以流形式播放
     * @param filename 文件路径
     * @return AndroidMusic实例
     */
    @Nullable
    Music newMusic(String filename);

    /**
     * 从Assets文件夹中加载声音，声音将被完整加载到内存，
     * 声音不应过长（5~6s），建议使用*.ogg格式
     * @param filename 文件路径
     * @return AndroidSound实例
     */
    Sound newSound(String filename);
}
