//
// Created by 李鼎立 on 2020/3/13.
//


#include "FFmpeg.h"

FFmpeg::~FFmpeg() {
    DELETE(dataSource);
    DELETE(javaCallHelper);
}

FFmpeg::FFmpeg(JavaCallHelper *javaCallHelper, char *dataSource) {
    this->javaCallHelper = javaCallHelper;
    this->dataSource = new char[strlen(dataSource) + 1];
    strcpy(this->dataSource, dataSource);
}

void *task_prepare(void *args) {
    FFmpeg *ffmpeg = static_cast<FFmpeg *>(args);
    ffmpeg->_prepare();
    return 0;
}

void FFmpeg::prepare() {
    pthread_create(&pid_prepare, 0, task_prepare, this);
}

void FFmpeg::_prepare() {
    avFormatContext = avformat_alloc_context();
    AVDictionary *dictionary = 0;
    av_dict_set(&dictionary, "timeout", "10000000", 0);
    int ret = avformat_open_input(&avFormatContext, dataSource, 0, 0);
    av_dict_free(&dictionary);
    if (ret) {
        LOGE("打开媒体失败：%s", av_err2str(ret));
        // todo
        return;
    }
    ret = avformat_find_stream_info(avFormatContext, 0);
    if (ret < 0) {
        // todo
        return;
    }

    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        AVStream *stream = avFormatContext->streams[i];
        AVCodecParameters *codecParameters = stream->codecpar;
        AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
        if (!codec) {
            // todo
            return;
        }

        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        ret = avcodec_parameters_to_context(codecContext, codecParameters);
        if (ret < 0) {
            // todo
            return;
        }
        ret = avcodec_open2(codecContext, codec, 0);
        if (ret) {
            // todo
            return;
        }

        if (codec->type == AVMEDIA_TYPE_AUDIO) {
            audioChannel = new AudioChannel(i, codecContext);
        } else if (codec->type == AVMEDIA_TYPE_VIDEO) {
            AVRational frame_rate = stream->avg_frame_rate;
            int fps = av_q2d(frame_rate);
            videoChannel = new VideoChannel(i, codecContext, fps);
            videoChannel->setRenderCallback(renderCallback);
        }
    }
    if (!audioChannel && !videoChannel) {
        // todo
        return;
    }

    if (javaCallHelper) {
        javaCallHelper->onPrepared(THREAD_CHILD);
    }
}

void *task_start(void *args) {
    FFmpeg *ffmpeg = static_cast<FFmpeg *>(args);
    ffmpeg->_start();
    return 0;
}

void FFmpeg::start() {
    isPlaying = 1;
    if (videoChannel) {
        videoChannel->start();
    }
    if (audioChannel) {
        audioChannel->start();
    }
    pthread_create(&pid_start, 0, task_start, this);
}

void FFmpeg::_start() {
    while (isPlaying) {
        if (videoChannel && videoChannel->packets.size() > 100) {
            av_usleep(10 * 1000);
            continue;
        }
        AVPacket *packet = av_packet_alloc();

        int ret = av_read_frame(avFormatContext, packet);
        if (!ret) {
            if (videoChannel && packet->stream_index == videoChannel->id) {
                videoChannel->packets.push(packet);
            } else if (audioChannel && packet->stream_index == audioChannel->id) {

            }
        } else if (ret == AVERROR_EOF) {
            // todo
        } else {
            // todo
            if (javaCallHelper) {
                javaCallHelper->onError(THREAD_CHILD, FFMPEG_READ_PACKET_FAIL);
            }
            break;
        }
    }
    isPlaying = 0;
    videoChannel->stop();
    audioChannel->stop();
}

void FFmpeg::setRenderCallback(RenderCallback renderCallback) {
    this->renderCallback = renderCallback;
}
