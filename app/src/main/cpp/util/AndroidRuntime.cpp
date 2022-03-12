//
// Created by wuzhanqiao on 2022/3/5.
//

#include "../include/util/AndroidRuntime.h"
#include <cassert>

JavaVM *AndroidRuntime::mJavaVM = nullptr;

void AndroidRuntime::setJavaVM(JavaVM *jvm) {
    mJavaVM = jvm;
}

JavaVM *AndroidRuntime::getJavaVM() {
    return mJavaVM;
}

JNIEnv *AndroidRuntime::getJNIEnv() {
    JNIEnv *env = nullptr;
    JavaVM *vm = AndroidRuntime::getJavaVM();
    assert(vm != nullptr);

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
        return nullptr;
    return env;
}
