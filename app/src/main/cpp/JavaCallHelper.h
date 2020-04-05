//
// Created by 李鼎立 on 2020/3/13.
//

#ifndef MY_NDK_APPLICATION_JAVACALLHELPER_H
#define MY_NDK_APPLICATION_JAVACALLHELPER_H


#include <jni.h>
#include "micro.h"

class JavaCallHelper {
public:
    JavaCallHelper(JavaVM *javaVm_, JNIEnv *env_, jobject instance_);

    ~JavaCallHelper();

    void onPrepared(int threadMode);

    void onError(int threadMode, int errorCode);

private:
    JavaVM *javaVm;
    JNIEnv *env;
    jobject instance;
    jmethodID jmd_prepared;
};


#endif //MY_NDK_APPLICATION_JAVACALLHELPER_H
