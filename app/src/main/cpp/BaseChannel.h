//
// Created by 李鼎立 on 2020/3/13.
//

#ifndef MY_NDK_APPLICATION_BASECHANNEL_H
#define MY_NDK_APPLICATION_BASECHANNEL_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
};

#include "safe_queue.h"

class BaseChannel {
public:
    BaseChannel(int id, AVCodecContext *codecContext) : id(id), codecContext(codecContext) {
        packets.setReleaseCallback(releaseAVPacket);
        frames.setReleaseCallback(releaseAVFrame);
    }

    virtual ~BaseChannel() {
        packets.clear();
        frames.clear();
    }

    static void releaseAVPacket(AVPacket **packet) {
        if (packet) {
            av_packet_free(packet);
            *packet = 0;
        }
    }

    static void releaseAVFrame(AVFrame **frame) {
        if (frame) {
            av_frame_free(frame);
            *frame = 0;
        }
    }

    virtual void start() = 0;

    virtual void stop() = 0;

    SafeQueue<AVPacket *> packets;
    SafeQueue<AVFrame *> frames;
    int id;
    bool isPlaying = 0;
    AVCodecContext *codecContext;
};

#endif //MY_NDK_APPLICATION_BASECHANNEL_H
