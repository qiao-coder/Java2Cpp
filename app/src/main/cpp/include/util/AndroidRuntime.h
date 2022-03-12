//
// Created by wuzhanqiao on 2022/3/5.
//

#ifndef JAVA2CPP_ANDROIDRUNTIME_H
#define JAVA2CPP_ANDROIDRUNTIME_H

#include <jni.h>

class AndroidRuntime {
public:
    static JNIEnv *getJNIEnv();

    static void setJavaVM(JavaVM *jvm);

    static JavaVM *getJavaVM();

private:
    static JavaVM *mJavaVM;
};


#endif //JAVA2CPP_ANDROIDRUNTIME_H
