//
// Created by 李鼎立 on 2020/3/13.
//

#include "AudioChannel.h"

AudioChannel::AudioChannel(int id, AVCodecContext *codecContext) : BaseChannel(id, codecContext) {}

AudioChannel::~AudioChannel() {

}

void *task_audio_decode(void *args) {
    AudioChannel *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->audio_decode();
    return 0;
}

void *task_audio_play(void *args) {
    AudioChannel *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->audio_play();
    return 0;
}

void AudioChannel::start() {
    isPlaying = 1;
    packets.setWork(1);
    frames.setWork(1);

    pthread_create(&pid_audio_decode, 0, task_audio_decode, this);
    pthread_create(&pid_audio_play, 0, task_audio_play, this);
}

void AudioChannel::stop() {

}

void AudioChannel::audio_decode() {
    AVPacket *packet = 0;
    while (isPlaying) {
        int ret = packets.pop(packet);
        if (!isPlaying) {
            break;
        }

        if (!ret) {
            continue;
        }

        ret = avcodec_send_packet(codecContext, packet);
        if (ret) {
            break;
        }
        releaseAVPacket(&packet);
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret != 0) {
            break;
        }
        while (isPlaying && frames.size() > 100) {
            av_usleep(10 * 1000);
            continue;
        }
        frames.push(frame);
    }
    releaseAVPacket(&packet);
}

void AudioChannel::audio_play() {
    SLresult result;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
}
