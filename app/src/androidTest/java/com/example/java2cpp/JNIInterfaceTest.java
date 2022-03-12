package com.example.java2cpp;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.java2cpp.bean.Image;
import com.example.java2cpp.bean.ImageFormat;
import com.example.java2cpp.bean.Position;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.nio.charset.StandardCharsets;

/**
 * @author wuzhanqiao
 * @date 2022/2/28.
 */
@RunWith(AndroidJUnit4.class)
public class JNIInterfaceTest {
    @Test
    public void helloCpp() {
        String s = "Hello from C++";
        assertEquals(s, JNIInterface.stringFromJNI());
    }

    @Test(expected = NullPointerException.class)
    public void testUnHandleJavaException() {
        JNIInterface.nativeUnHandleJavaException();
    }

    @Test
    public void testHandleJavaException() {
        assertEquals(-1, JNIInterface.nativeHandleJavaException());
    }

    @Test
    public void testHandleJavaExceptionTooLate() {

    }

    @Test
    public void testDynamicRegisterMethod() {
        assertEquals(0, new JNIInterface().nativeDynamicRegisterMethod());
    }

    /**
     * JNI ERROR (app bug): local reference table overflow (max=8388608)
     */
    @Test
    public void testCreateLocalRefTooMuch() {
//        JNIInterface.nativeCreateLocalRefTooMuch();
    }

    @Test
    public void testCreateLocalRefAndDeleteTimelyInTraversal() {
        JNIInterface.nativeCreateLocalRefAndDeleteTimelyInTraversal();
    }

    @Test
    public void testCreateGlobalRefTooMuch() {
//        JNIInterface.nativeCreateGlobalRefTooMuch();
    }

    @Test
    public void testTransmitPrimitiveType() {
        JNIInterface.nativeTransmitPrimitiveType(Integer.MAX_VALUE, Long.MAX_VALUE, Float.MAX_VALUE,
                Byte.MAX_VALUE, Double.MAX_VALUE, true, Short.MAX_VALUE, '中');
    }

    @Test
    public void testFillImage() {
        Image image = new Image();
        byte[] data = image.data;
        byte[] extraData = image.extraData;
        JNIInterface.nativeFillImage(image);

        assertEquals(999, image.id);
        assertEquals(1920, image.width);
        assertEquals(1080, image.height);
        assertEquals(9.9f, image.pos.longitude, 0);
        assertEquals(99.9f, image.pos.latitude, 0);


        //data在C++层被赋予了新的值。引用发生了变化。
        assertNotEquals(data, image.data);
        for (int i = 0; i < image.data.length; i++) {
            assertEquals(i, image.data[i]);
        }

        //extra没有发生变化，只是修改了值
        assertEquals(extraData, image.extraData);
        for (int i = 0; i < image.extraData.length; i++) {
            assertEquals(i, image.extraData[i]);
        }
    }

    @Test
    public void testTransmitEnum() {
        ImageFormat returnFormat = JNIInterface.nativeTransmitEnum(ImageFormat.NV21.ordinal(), ImageFormat.NV21);
        assertEquals(ImageFormat.RGB_888, returnFormat);
    }

    @Test
    public void testTransmitString() {
        assertEquals("中国", JNIInterface.nativeTransmitString("中国"));
    }

    @Test
    public void testGetGB2312() {
        assertEquals("中国", JNIInterface.nativeGetGB2312String());
    }

    /**
     * Unicode和UTF编码转换网站：@see https://www.qqxiuzi.cn/bianma/Unicode-UTF.php
     * 在Dalvik上，即4.4及以上（4.4支持Art，但默认使用Dalvik），没有对4字节utf-8编码的字符串进行处理。测试不通过。
     * 在Art上，即5.0及以上，针对4字节编码的utf-8会转成代理对处理。测试通过。
     */
    @Test
    public void testGet4byteUTF8() {
        //"\uD843\uDC96"即"𠲖"，这里被studio自动转码成了unicode显示。
        //"𠲖"的utf-8编码为F0A0B296
        "a".getBytes(StandardCharsets.UTF_8);
        String s = Integer.toHexString(0xc080);
        byte[] b = {0};

        assertEquals("\uD843\uDC96", JNIInterface.nativeGet4ByteUTF8String("\uD843\uDC96"));
    }

    @Test
    public void testCallJavaMethodInNative() {
        Image image = new Image();

        Position position = JNIInterface.nativeGetImagePos(image);
        assertEquals(image.pos, position);

        byte[] data = JNIInterface.nativeGetImageData(image);
        assertEquals(image.data, data);

        int result = JNIInterface.nativeAdd(1, 2);
        assertEquals(3, result);
    }
}
