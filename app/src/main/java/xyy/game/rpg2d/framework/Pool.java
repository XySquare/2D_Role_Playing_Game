package xyy.game.rpg2d.framework;

import java.util.ArrayList;
import java.util.List;

/**
 * 实例池（instance pooling）
 * 回收以前创建的实例，优化垃圾回收
 * Created by ${XYY} on ${2016/2/13}.
 */
public final class Pool<T> {
    /**
     * 返回新/重用实例T
     * @param <T>
     */
    public interface PoolObjectFactory<T> {
        T createObject();
    }

    //储存以入池的对象
    private final List<T> freeObjects;
    //生成实例
    private final PoolObjectFactory<T> factory;
    //池可容纳对象的最大数量
    private final int maxSize;

    /**
     * 实例池（instance pooling）
     * @param factory 生成实例
     * @param maxSize 可容纳对象的最大数量
     */
    public Pool(PoolObjectFactory<T> factory, int maxSize) {
        this.factory = factory;
        this.maxSize = maxSize;
        this.freeObjects = new ArrayList<>(maxSize);
    }

    /**
     * 通过PoolObjectFactory.createObject()方法创建新实例，或重用实例
     * @return 实例T
     */
    public T newObject() {
        T object;

        if (freeObjects.size() == 0)
            object = factory.createObject();
        else
            object = freeObjects.remove(freeObjects.size() - 1);

        return object;
    }

    /**
     * 如果freelist没填满，则插入不用的对象
     * @param object 实例T
     */
    public void free(T object) {
        if (freeObjects.size() < maxSize)
            freeObjects.add(object);
    }
}
