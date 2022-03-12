package com.example.java2cpp;

import com.example.java2cpp.bean.Image;
import com.example.java2cpp.bean.ImageFormat;
import com.example.java2cpp.bean.Position;

/**
 * @author wuzhanqiao
 * @date 2022/2/8.
 */
class JNIInterface {
    public int num = 1;
    public static int staticNum = 2;

    private int getNum() {
        return num;
    }

    private static int getStaticNum() {
        return staticNum;
    }

    static {
//        System.load("/data/data/com.example.java2cpp/lib/libjava2cpp.so");
//        load和loadLibrary是等价的。
        System.loadLibrary("java2cpp");

    }

    private static void javaThrowException() {
        throw new NullPointerException();
    }

    public native static String stringFromJNI();

    public native void nativeJniMethod();

    public native static void nativeStaticJniMethod();

    public native int nativeDynamicRegisterMethod();

    public native static void nativeCreateLocalRefTooMuch();

    public native static void nativeCreateLocalRefAndDeleteTimelyInTraversal();

    public native static void nativeCreateGlobalRefTooMuch();

    public native static void nativeFillImage(Image image);

    public native static void nativeTransmitPrimitiveType(int i, long l, float f,
                                                          byte _byte, double d, boolean b, short s, char c);

    public native static Position nativeGetImagePos(Image image);

    public native static byte[] nativeGetImageData(Image image);

    public native static int nativeAdd(int i, int j);

    public native static ImageFormat nativeTransmitEnum(int iFormat, ImageFormat format);

    public static native String nativeTransmitString(String s);

    public native static String nativeGetGB2312String();

    public native static String nativeGet4ByteUTF8String(String s);

    public native static int nativeUnHandleJavaException();

    public native static int nativeHandleJavaException();
}
