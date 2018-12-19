package xyy.game.rpg2d.framework;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * FileI/O接口
 * Created by ${XYY} on ${2015/11/20}.
 */
public interface FileIO {
    String getExternalStoragePath();

    /**
     * 从Assets文件夹读取资源文件
     * @param fileName 文件路径
     * @return 输入数据流
     * @throws IOException
     */
    //InputStream readAsset(String fileName) throws IOException;

    /**
     * 读取SD卡上的文件
     * @param fileName 文件路径
     * @return 输入数据流
     * @throws IOException
     */
    InputStream readFile(String fileName) throws IOException;

    /**
     * 写入SD卡上的文件
     * @param fileName 文件路径
     * @return 输出数据流
     * @throws IOException
     */
    OutputStream writeFile(String fileName) throws IOException;

    //OutputStream appendFile(String fileName) throws IOException;

    /**
     * 删除SD卡上的文件
     * @param fileName 文件路径
     * @return true:删除成功
     *          false:文件不存在 或 删除失败
     */
    boolean deleteFile(String fileName);

    boolean exists(String fileName);

    File[] listFIle(String pathName);
}
