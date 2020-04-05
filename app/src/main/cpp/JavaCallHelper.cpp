//
// Created by 李鼎立 on 2020/3/13.
//

#include "JavaCallHelper.h"

JavaCallHelper::JavaCallHelper(JavaVM *javaVm_, JNIEnv *env_, jobject instance_) {
    this->javaVm = javaVm_;
    this->env = env_;
    this->instance =env->NewGlobalRef(instance_);
    jclass clazz = env->GetObjectClass(instance);
    jmd_prepared = env->GetMethodID(clazz, "onPrepared", "()V");
}

JavaCallHelper::~JavaCallHelper() {
    DELETE(javaVm);
    env->DeleteGlobalRef(instance);
    instance = 0;
}

void JavaCallHelper::onPrepared(int threadMode) {
    if (threadMode == THREAD_MAIN) {
        env->CallVoidMethod(instance, jmd_prepared);
    } else {
        JNIEnv *env_child;
        javaVm->AttachCurrentThread(&env_child, 0);
        env_child->CallVoidMethod(instance, jmd_prepared);
        javaVm->DetachCurrentThread();
    }
}

void JavaCallHelper::onError(int threadMode, int errorCode) {

}
