package com.example.java2cpp.bean;

import java.util.Arrays;

/**
 * @author wuzhanqiao
 * @date 2022/2/20.
 */
public class Image {
    private static final String TAG = "Image";
    public long id;
    public int width;
    public int height;

    public Position pos = new Position();

    public byte[] data = new byte[10];

    public byte[] extraData = new byte[20];

    public Position getPos() {
        return pos;
    }

    private byte[] getData() {
        return data;
    }

    @Override
    public String toString() {
        return "Image{" +
                "id=" + id +
                ", width=" + width +
                ", height=" + height +
                ", data=" + Arrays.toString(data) +
                '}';
    }
}
