//
// Created by wuzhanqiao on 2022/3/5.
//

#include <jni.h>
#include <android/log.h>
#include <cassert>
#include "include/util/AndroidRuntime.h"
#include "mutex"
#include "include/media/mediaplayer.h"

struct JavaMediaPlayer {
    jclass class_ref;
    jfieldID native_context;
    jmethodID post_event;
};
JavaMediaPlayer javaMediaPlayer;

static std::mutex sLock;


class JNIMediaPlayerListener : public MediaPlayerListener {
public:
    JNIMediaPlayerListener(JNIEnv *env, jobject thiz, jobject weak_thiz);

    ~JNIMediaPlayerListener();

    virtual void notify(int msg);

private:
    jclass mClass;     // Reference to MediaPlayer class
    jobject mObject;
};

JNIMediaPlayerListener::JNIMediaPlayerListener(JNIEnv *env, jobject thiz, jobject weak_thiz) {
    jclass c = env->GetObjectClass(thiz);
    if (c == nullptr) {
        const char *msg = "Can't find com/example/java2cpp/MediaPlayer";
        env->FatalError(msg);
    }

    mClass = (jclass) env->NewGlobalRef(c);
    mObject = env->NewGlobalRef(weak_thiz);
}

JNIMediaPlayerListener::~JNIMediaPlayerListener() {
    JNIEnv *env = AndroidRuntime::getJNIEnv();
    if (env) {
        env->DeleteGlobalRef(mObject);
        env->DeleteGlobalRef(mClass);
    }
}

void JNIMediaPlayerListener::notify(int msg) {
    JNIEnv *env;
    JavaVM *vm = AndroidRuntime::getJavaVM();
    if (vm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        return;
    }
    env->CallStaticVoidMethod(mClass, javaMediaPlayer.post_event, mObject, msg);
    //使用完，记得Detach
    vm->DetachCurrentThread();
}

static void setMediaPlayer(JNIEnv *env, jobject thiz, MediaPlayer *player) {
    std::unique_lock<std::mutex> lock(sLock);
    //指针要强转为jlong
    env->SetLongField(thiz, javaMediaPlayer.native_context, reinterpret_cast<jlong>(player));
}

static MediaPlayer *getMediaPlayer(JNIEnv *env, jobject thiz) {
    std::unique_lock<std::mutex> lock(sLock);
    return reinterpret_cast<MediaPlayer *>(env->GetLongField(thiz, javaMediaPlayer.native_context));
}

void android_media_MediaPlayer_native_setup(JNIEnv *env, jobject thiz, jobject weak_thiz) {
    MediaPlayer *player = new MediaPlayer();
    JNIMediaPlayerListener *listener = new JNIMediaPlayerListener(env, thiz, weak_thiz);
    player->setListener(listener);
    setMediaPlayer(env, thiz, player);
}


void android_media_MediaPlayer_finalize(JNIEnv *env, jobject thiz) {
    MediaPlayer *player = getMediaPlayer(env, thiz);
    //listener的释放时机？在MediaPlayer的析构函数里。
    if (player) delete player;
    setMediaPlayer(env, thiz, 0);
}

void android_media_MediaPlayer_prepareAsync(JNIEnv *env, jobject thiz) {
    MediaPlayer *player = getMediaPlayer(env, thiz);
    player->prepareAsync();
}

static const JNINativeMethod gMethods[] = {
        {"native_setup",    "(Ljava/lang/Object;)V", (void *) android_media_MediaPlayer_native_setup},
        {"native_finalize", "()V",                   (void *) android_media_MediaPlayer_finalize},
        {"prepareAsync",    "()V",                   (void *) android_media_MediaPlayer_prepareAsync},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    AndroidRuntime::setJavaVM(vm);
    JNIEnv *env = nullptr;
    jint result = JNI_ERR;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    assert(env != nullptr);

    jclass c = env->FindClass("com/example/java2cpp/MediaPlayer");

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

    javaMediaPlayer.class_ref = reinterpret_cast<jclass>(env->NewGlobalRef(c));
    javaMediaPlayer.native_context = env->GetFieldID(c, "mNativeContext", "J");
    javaMediaPlayer.post_event = env->GetStaticMethodID(c, "postEventFromNative",
                                                        "(Ljava/lang/Object;I)V");
    env->DeleteLocalRef(c);

    return JNI_VERSION_1_6;
}

/**
 *
 * 程序退出时，并不会调用JNI_OnUnload
 */
void JNI_OnUnload(JavaVM *vm, void *reserved) {
    AndroidRuntime::setJavaVM(vm);
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    jclass c = env->FindClass("com/example/java2cpp/MediaPlayer");
    if (env->UnregisterNatives(c) < 0) {
        const char *msg = "UnregisterNatives failed; aborting...";
        env->FatalError(msg);
    }
}