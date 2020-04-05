//
// Created by 李鼎立 on 2020/3/13.
//

#ifndef MY_NDK_APPLICATION_AUDIOCHANNEL_H
#define MY_NDK_APPLICATION_AUDIOCHANNEL_H


#include "BaseChannel.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class AudioChannel : public BaseChannel{
public:
    AudioChannel(int id, AVCodecContext *codecContext);

    ~AudioChannel();

    void start();

    void stop();

    void audio_decode();

    void audio_play();

private:
    pthread_t pid_audio_decode;
    pthread_t pid_audio_play;
    SLObjectItf engineObject = 0;
    SLEngineItf engineInterface = 0;
};


#endif //MY_NDK_APPLICATION_AUDIOCHANNEL_H
