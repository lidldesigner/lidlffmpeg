//
// Created by 李鼎立 on 2020/3/13.
//

#ifndef MY_NDK_APPLICATION_MICRO_H
#define MY_NDK_APPLICATION_MICRO_H

#include <android/log.h>

#define DELETE(object) if(object){delete object; object=0;}

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FFMPEG",__VA_ARGS__)

#define THREAD_MAIN 1
#define THREAD_CHILD 2

#define FFMPEG_READ_PACKET_FAIL 1002

#endif //MY_NDK_APPLICATION_MICRO_H
