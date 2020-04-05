//
// Created by 李鼎立 on 2020/3/13.
//

#ifndef MY_NDK_APPLICATION_FFMPEG_H
#define MY_NDK_APPLICATION_FFMPEG_H


#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"
#include <cstring>
#include "micro.h"
#include <pthread.h>
extern "C"{
#include <libavformat/avformat.h>
}

class FFmpeg {
public:

    FFmpeg(JavaCallHelper *javaCallHelper, char *dataSource);

    ~FFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();

    void setRenderCallback(RenderCallback renderCallback);

private:
    JavaCallHelper *javaCallHelper = 0;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    char *dataSource;
    pthread_t pid_prepare;
    pthread_t pid_start;
    bool isPlaying;
    AVFormatContext *avFormatContext = 0;
    RenderCallback renderCallback;
};


#endif //MY_NDK_APPLICATION_FFMPEG_H
