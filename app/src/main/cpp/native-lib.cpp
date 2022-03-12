#include <jni.h>
#include <string>
#include "android/log.h"
#include <limits>


#ifndef TAG
#define TAG "JNI"
#endif

#ifndef JNIInterface_CLASS
#define JNIInterface_CLASS "com/example/java2cpp/JNIInterface"
#endif

using std::numeric_limits;

enum image_format {
    RGB_888, NV21, NV12
};

struct JavaJNIInterface {
    jclass class_ref;
    jfieldID num;
    jfieldID static_num;
    jmethodID getNum;
    jmethodID getStaticNum;
};
static JavaJNIInterface javaJNIInterface;

struct JavaImage {
    jclass class_ref;
    jfieldID id;
    jfieldID width;
    jfieldID height;
    jfieldID position;
    jfieldID data;
    jfieldID extra_data;
    jmethodID getPos;
    jmethodID getData;
};
static JavaImage javaImage;

struct JavaPosition {
    jclass class_ref;
    jfieldID longitude;
    jfieldID latitude;
};
static JavaPosition javaPosition;

struct JavaImageFormat {
    jclass class_ref;
    jmethodID values;
    jmethodID name;
    jmethodID ordinal;
};
static JavaImageFormat javaImageFormat;

struct JavaCalculator {
    jclass class_ref;
    jmethodID add;
};
JavaCalculator javaCalculator;

struct JavaString {
    jclass class_ref;
    jmethodID constructor;
};
JavaString javaString;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_java2cpp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_java2cpp_JNIInterface_nativeJniMethod(JNIEnv *env, jobject thiz) {

}
extern "C" JNIEXPORT void JNICALL
Java_com_example_java2cpp_JNIInterface_nativeStaticJniMethod(JNIEnv *env, jclass clazz) {

}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_java2cpp_JNIInterface_stringFromJNI(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_java2cpp_JNIInterface_nativeUnHandleJavaException(JNIEnv *env, jclass clazz) {
    jclass javaJNIClass = env->FindClass("com/example/java2cpp/JNIInterface");
    jmethodID javaThrowExceptMethod = env->GetStaticMethodID(clazz, "javaThrowException", "()V");
    env->CallStaticVoidMethod(javaJNIClass, javaThrowExceptMethod);
    if (env->ExceptionCheck()) {
//        env->ExceptionClear();
        return -1;
    } else {
        return 0;
    }
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_java2cpp_JNIInterface_nativeHandleJavaException(JNIEnv *env, jclass clazz) {
    jclass javaJNIClass = env->FindClass("com/example/java2cpp/JNIInterface");
    jmethodID javaThrowExceptMethod = env->GetStaticMethodID(clazz, "javaThrowException", "()V");
    //产生了一个Java异常
    env->CallStaticVoidMethod(javaJNIClass, javaThrowExceptMethod);
    //在Clear之前，不能调用除Check、Clear之外的方法
    //env->FindClass("com/example/java2cpp/JNI");
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        return -1;
    } else {
        return 0;
    }
}

static jint dynamic_register_method(JNIEnv *env, jobject thiz) {
    return 0;
}


static void locate_java_element(JNIEnv *env, jclass clazz) {
    jclass c = env->FindClass("com/example/java2cpp/JNIInterface");
    jclass global_ref = reinterpret_cast<jclass>( env->NewGlobalRef(c));
    jfieldID num = env->GetFieldID(c, "num", "I");
    jfieldID static_num = env->GetStaticFieldID(c, "staticNum", "I");
    jmethodID getNum = env->GetMethodID(c, "getNum", "()I");
    jmethodID getStaticNum = env->GetStaticMethodID(c, "getStaticNum", "()I");
}


static void create_local_ref_too_much(JNIEnv *env, jclass clazz) {
    for (int i = 0; i < 10000000; i++) {
        jclass c = env->FindClass(JNIInterface_CLASS);
    }
}

static void create_local_ref_and_delete_timely_in_traversal(JNIEnv *env, jclass clazz) {
    for (int i = 0; i < 10000000; i++) {
        jclass c = env->FindClass(JNIInterface_CLASS);
        env->DeleteLocalRef(c);
    }
}

static void create_global_ref_too_much(JNIEnv *env, jclass clazz) {
    for (int i = 0; i < 10000000; i++) {
        jclass c = env->FindClass(JNIInterface_CLASS);
        jclass global_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
        env->DeleteLocalRef(c);
    }
}

static void transmit_primitive_type(JNIEnv *env, jclass clazz, jint i, jlong l, jfloat f,
                                    jbyte byte, jdouble d, jboolean b, jshort s, jchar c) {
    int c_i = numeric_limits<int>::max();
    assert(i == c_i);
    //long等同于long int
    assert(i == numeric_limits<long>::max());

    long long c_l = numeric_limits<long long>::max();
    assert(l == c_l);
    assert(l != numeric_limits<unsigned long long>::max());
    assert(l == numeric_limits<long long int>::max());

    float c_f = numeric_limits<float>::max();
    assert(f == c_f);

    signed char c_byte = numeric_limits<signed char>::max();
    assert(byte == c_byte);
    //不恰当的比较
    //assert(byte == numeric_limits<char>::max());
    assert(255 == numeric_limits<unsigned char>::max());

    double c_d = numeric_limits<double>::max();
    assert(d == c_d);

    bool c_b = numeric_limits<bool>::max();
    assert(b == c_b);

    short c_s = numeric_limits<short>::max();
    assert(s == c_s);

    unsigned short c_c = 0x4e2d;//'中'的Unicode编码
    assert(2 == sizeof(unsigned short));
    assert(c == c_c);
}

static void fill_image(JNIEnv *env, jclass clazz, jobject image) {
    //获取对象的基本类型变量：通过Get<PrimitiveType>Field系列方法
    jlong id = env->GetLongField(image, javaImage.id);
    jint width = env->GetIntField(image, javaImage.width);
    jint height = env->GetIntField(image, javaImage.height);

    assert(id == 0);
    assert(width == 0);
    assert(height == 0);

    //获取对象的普通引用类型变量：通过GetObjectField
    jobject position = env->GetObjectField(image, javaImage.position);
    if (position != nullptr) {
        jfloat longitude = env->GetFloatField(position, javaPosition.longitude);
        jfloat latitude = env->GetFloatField(position, javaPosition.latitude);
        assert(longitude == 0.0f);
        assert(latitude == 0.0f);
    }

    //获取对象的数组变量：也是通过GetObjectField，需要强转为对应的JNI引用类型，如：
    //byte[]<->jbyteArray
    //int[]<->jintArray
    jbyteArray data = reinterpret_cast<jbyteArray>( env->GetObjectField(image, javaImage.data));
    //获取Java数组内容
    if (data != nullptr) {
        jsize size = env->GetArrayLength(data);
        jboolean isCopy;
        //基本类型数组：使用Get<PrimitiveType>ArrayElements系列方法。
        //引用类型数组：使用GetObjectArrayElement
        jbyte *byte_array_native = env->GetByteArrayElements(data, &isCopy);
        for (int i = 0; i < size; ++i) {
            jbyte b = byte_array_native[i];
            assert(b == 0);
        }
        //第三个参数mode通常为0即可
        env->ReleaseByteArrayElements(data, byte_array_native, 0);
    }

    //修改对象的基本类型变量：通过Set<PrimitiveType>Field系列方法
    env->SetLongField(image, javaImage.id, 999);
    env->SetIntField(image, javaImage.width, 1920);
    env->SetIntField(image, javaImage.height, 1080);

    //修改对象的普通引用类型变量：通过SetObjectField
    //创建一个Java对象：通过AllocObject
    jobject newPosition = env->AllocObject(javaPosition.class_ref);
    env->SetFloatField(newPosition, javaPosition.longitude, 9.9f);
    env->SetFloatField(newPosition, javaPosition.latitude, 99.9f);
    env->SetObjectField(image, javaImage.position, newPosition);

    //修改对象的数组类型变量：也是通过SetObjectField
    //创建Java基本类型的数组：通过New<PrimitiveType>Array系列方法
    //创建Java引用类型的数组：通过NewObjectArray
    jbyteArray newData = env->NewByteArray(10);
    int len = 1;
    for (int i = 0; i < 10; ++i) {
        jbyte b = i;
        //基本类型数组通过Set<PrimitiveType>ArrayRegion系列方法设置数组的元素
        //Set<PrimitiveType>ArrayRegion其实可以直接设置整个C++数组给newData;
        //如果是引用类型数组，则要通过SetObjectArrayElement一个个设置数组的元素
        env->SetByteArrayRegion(newData, i, len, &b);
    }
    env->SetObjectField(image, javaImage.data, newData);


    //直接修改Java数组的内容：大体流程与获取Java数组内容一致
    jbyteArray extraData = reinterpret_cast<jbyteArray>(env->GetObjectField(image,
                                                                            javaImage.extra_data));
    if (data != nullptr) {
        jsize size = env->GetArrayLength(extraData);
        jboolean isCopy;
        jbyte *byte_array_native = env->GetByteArrayElements(extraData, &isCopy);
//        assert(isCopy == JNI_FALSE);
        for (int i = 0; i < size; i++) {
            //更改数组的内容
            byte_array_native[i] = i;
        }
        //第三个参数：mode，有三种：
        //0 : 当isCopy为true时，将缓冲数组的内容拷贝回原数组，释放缓冲数组。
        //JNI_COMMIT : 当isCopy为true时，将缓冲数组的内容拷贝回原数组，不会释放缓冲数组。后续得用户自己调用delete[]释放缓存数组。
        //JNI_ABORT : 当isCopy为true，不会将缓冲数组的内容拷贝回原数组，释放缓冲数组。

        //如果isCopy为false。将不存在缓冲数组，即byte_array_native直接指向Java数组所在的内存。修改byte_array_native，等于直接修改Java原数组。
        //这时候，三种mode，不起任何作用。

        //将缓冲数组的内容拷贝回原数组，释放缓冲数组。
        env->ReleaseByteArrayElements(extraData, byte_array_native, 0);
    }
}

static jobject get_image_pos(JNIEnv *env, jclass clazz, jobject image) {
    jobject pos = env->CallObjectMethod(image, javaImage.getPos);
    return pos;
}

static jbyteArray get_image_data(JNIEnv *env, jclass clazz, jobject image) {
    jbyteArray data = reinterpret_cast<jbyteArray> (env->CallObjectMethod(image,
                                                                          javaImage.getData));
    return data;
}

static jint add(JNIEnv *env, jclass clazz, jint i, jint j) {
    jint result = env->CallStaticIntMethod(javaCalculator.class_ref, javaCalculator.add, i, j);
    return result;
}

static jobject transmit_enum(JNIEnv *env, jclass clazz, jint i_format, jobject format) {
    //如果传的是java枚举的下标
    //c++的枚举，可以自动转为int
    assert(i_format == image_format::NV21);

    //int无法自动转换为c++枚举，需要强转
    image_format nv21 = static_cast<image_format>(i_format);
    assert(nv21 == image_format::NV21);

    //调用Java枚举的ordinal方法，获取对应枚举的下标
    image_format c_format = static_cast<image_format> (env->CallIntMethod(format,
                                                                          javaImageFormat.ordinal));
    assert(c_format == image_format::NV21);


    image_format c_rgb = image_format::RGB_888;
    //调用Java枚举的静态方法values，获取枚举集合
    jobjectArray values = reinterpret_cast<jobjectArray>(env->CallStaticObjectMethod(
            javaImageFormat.class_ref, javaImageFormat.values));
    //获取对应的Java枚举
    jobject java_format = env->GetObjectArrayElement(values, c_rgb);
    return java_format;
}

static jstring transmit_string(JNIEnv *env, jclass clazz, jstring s) {
    jboolean isCopy;
    const char *chars = env->GetStringUTFChars(s, &isCopy);
    std::string c_s = "中国";
    assert(strcmp(chars, c_s.c_str()) == 0);

    char c_chars[7];
    c_chars[0] = 0xE4;//E4B8AD即"中"的UTF-8编码
    c_chars[1] = 0xB8;
    c_chars[2] = 0xAD;
    c_chars[3] = 0xE5;//E59BBD即"国"的UTF-8编码
    c_chars[4] = 0x9B;
    c_chars[5] = 0xBD;
    c_chars[6] = '\0';//'\0'，C语言里用来表示字符串的结尾

    assert(strcmp(chars, c_chars) == 0);

    //使用完，要及时释放
    env->ReleaseStringUTFChars(s, chars);

    return env->NewStringUTF(c_chars);
}


static jstring native_get_4ByteUTF8String(JNIEnv *env, jclass clazz, jstring s) {
    jboolean isCopy;
    const char *c = env->GetStringUTFChars(s, &isCopy);


    char c_str[5];
    c_str[0] = 0xF0;//“𠲖”,前4个字节表示𠲖的UTF8编码
    c_str[1] = 0xA0;
    c_str[2] = 0xB2;
    c_str[3] = 0x96;
    c_str[4] = 0x00;//即'\0'，C语言里用来表示字符串的结尾

    char c_null[3]{static_cast<char>(0xc0), static_cast<char>(0x80), 0};
    assert(strcmp(c, c_str) == 0);

    env->ReleaseStringUTFChars(s, c);

    return env->NewStringUTF(c_str);
}

static jstring native_get_GB2312String(JNIEnv *env, jclass clazz) {
    char c_str[5];
    c_str[0] = 0xD6;//D6D0即"中"
    c_str[1] = 0xD0;
    c_str[2] = 0xB9;//B9FA即"国"
    c_str[3] = 0xFA;
    c_str[4] = 0x00;//即'\0'，C语言里用来表示字符串的结尾


    jbyteArray bytes = env->NewByteArray((jsize) strlen(c_str));
    env->SetByteArrayRegion(bytes, 0, (jsize) strlen(c_str), (jbyte *) c_str);
    jstring encoding = env->NewStringUTF("gb2312");

    return reinterpret_cast<jstring>(env->NewObject(javaString.class_ref, javaString.constructor,
                                                    bytes, encoding));
}

/**
 * 代表native方法声明与native方法实现映射关系
 * 结构体JNINativeMethod的三个字段分别是：
 * name：Java定义的native方法名
 * signature：Java定义的native方法的类型签名
 * fnPtr：native方法的函数指针，指向native方法的实现
 */
static const JNINativeMethod gMethods[] = {
        {"nativeDynamicRegisterMethod",                    "()I",                                                                               (void *) dynamic_register_method},
        {"nativeCreateLocalRefTooMuch",                    "()V",                                                                               (void *) create_local_ref_too_much},
        {"nativeCreateLocalRefAndDeleteTimelyInTraversal", "()V",                                                                               (void *) create_local_ref_and_delete_timely_in_traversal},
        {"nativeCreateGlobalRefTooMuch",                   "()V",                                                                               (void *) create_global_ref_too_much},
        {"nativeFillImage",                                "(Lcom/example/java2cpp/bean/Image;)V",                                              (void *) fill_image},
        {"nativeTransmitPrimitiveType",                    "(IJFBDZSC)V",                                                                       (void *) transmit_primitive_type},
        {"nativeTransmitEnum",                             "(ILcom/example/java2cpp/bean/ImageFormat;)Lcom/example/java2cpp/bean/ImageFormat;", (void *) transmit_enum},
        {"nativeGetImagePos",                              "(Lcom/example/java2cpp/bean/Image;)Lcom/example/java2cpp/bean/Position;",           (void *) get_image_pos},
        {"nativeGetImageData",                             "(Lcom/example/java2cpp/bean/Image;)[B",                                             (void *) get_image_data},
        {"nativeAdd",                                      "(II)I",                                                                             (void *) add},
        {"nativeTransmitString",                           "(Ljava/lang/String;)Ljava/lang/String;",                                            (void *) transmit_string},
        {"nativeGet4ByteUTF8String",                       "(Ljava/lang/String;)Ljava/lang/String;",                                            (void *) native_get_4ByteUTF8String},
        {"nativeGetGB2312String",                          "()Ljava/lang/String;",                                                              (void *) native_get_GB2312String},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_INFO, TAG, "%s", "JNI_OnLoad");
    JNIEnv *env = nullptr;
    jint result = JNI_ERR;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    assert(env != nullptr);

    jclass c = env->FindClass(JNIInterface_CLASS);


    if (c == nullptr) {
        const char *msg = "Native registration unable to find class; aborting...";
        env->FatalError(msg);
    }

    int numMethods = sizeof(gMethods) / sizeof(gMethods[0]);
    //采用RegisterNatives进行动态注册
    if (env->RegisterNatives(c, gMethods, numMethods) < 0) {
        const char *msg = "RegisterNatives failed; aborting...";
        env->FatalError(msg);
    }

    //Java元素定位
    javaJNIInterface.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaJNIInterface.num = env->GetFieldID(c, "num", "I");
    javaJNIInterface.static_num = env->GetStaticFieldID(c, "staticNum", "I");
    javaJNIInterface.getNum = env->GetMethodID(c, "getNum", "()I");
    javaJNIInterface.getStaticNum = env->GetStaticMethodID(c, "getStaticNum", "()I");
    //本地引用在native方法完成调用后，自动移除。大部分时候DeleteLocalRef可有可无。
    env->DeleteLocalRef(c);

    c = env->FindClass("com/example/java2cpp/bean/Image");
    javaImage.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaImage.id = env->GetFieldID(c, "id", "J");
    javaImage.width = env->GetFieldID(c, "width", "I");
    javaImage.height = env->GetFieldID(c, "height", "I");
    javaImage.position = env->GetFieldID(c, "pos", "Lcom/example/java2cpp/bean/Position;");
    javaImage.data = env->GetFieldID(c, "data", "[B");
    javaImage.extra_data = env->GetFieldID(c, "extraData", "[B");
    javaImage.getPos = env->GetMethodID(c, "getPos", "()Lcom/example/java2cpp/bean/Position;");
    javaImage.getData = env->GetMethodID(c, "getData", "()[B");
    env->DeleteLocalRef(c);

    c = env->FindClass("com/example/java2cpp/bean/Position");
    javaPosition.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaPosition.latitude = env->GetFieldID(c, "latitude", "F");
    javaPosition.longitude = env->GetFieldID(c, "longitude", "F");
    env->DeleteLocalRef(c);


    c = env->FindClass("com/example/java2cpp/bean/ImageFormat");
    javaImageFormat.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaImageFormat.name = env->GetMethodID(c, "name", "()Ljava/lang/String;");
    javaImageFormat.ordinal = env->GetMethodID(c, "ordinal", "()I");
    javaImageFormat.values = env->GetStaticMethodID(c, "values",
                                                    "()[Lcom/example/java2cpp/bean/ImageFormat;");
    env->DeleteLocalRef(c);

    c = env->FindClass("com/example/java2cpp/util/Calculator");
    javaCalculator.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaCalculator.add = env->GetStaticMethodID(c, "add", "(II)I");
    env->DeleteLocalRef(c);

    c = env->FindClass("java/lang/String");
    javaString.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    //java.lang.String的一个构造方法
    javaString.constructor = env->GetMethodID(c, "<init>", "([BLjava/lang/String;)V");
    env->DeleteLocalRef(c);

    return JNI_VERSION_1_6;
}

/**
 *
 * 程序退出时，并不会调用JNI_OnUnload
 */
void JNI_OnUnload(JavaVM *vm, void *reserved) {
    __android_log_print(ANDROID_LOG_INFO, TAG, "%s", "JNI_OnUnload");
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    jclass c = env->FindClass(JNIInterface_CLASS);
    if (env->UnregisterNatives(c) < 0) {
        const char *msg = "UnregisterNatives failed; aborting...";
        env->FatalError(msg);
    }
}