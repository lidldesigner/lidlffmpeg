#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "FFmpeg.h"
#include <android/native_window_jni.h>

extern "C" {
#include <libavutil/avutil.h>
}
JavaVM *javaVm = 0;
JavaCallHelper *javaCallHelper = 0;
FFmpeg *ffmpeg = 0;
ANativeWindow *window;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态初始化mutex

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myndkapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
    return env->NewStringUTF(av_version_info());
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVm = vm;
    return JNI_VERSION_1_4;
}

void renderFrame(uint8_t *src_data, int src_linesize, int width, int height) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(window, &windowBuffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }
    uint8_t *dst_data = static_cast<uint8_t *>(windowBuffer.bits);
    int dst_linesize = windowBuffer.stride * 4;
    for (int i = 0; i < windowBuffer.height; ++i) {
        memcpy(dst_data + i * dst_linesize, src_data + i * src_linesize, dst_linesize);
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myndkapplication_Player_prepareNative(JNIEnv *env, jobject thiz,
                                                       jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, 0);

    javaCallHelper = new JavaCallHelper(javaVm, env, thiz);
    ffmpeg = new FFmpeg(javaCallHelper, const_cast<char *>(dataSource));
    ffmpeg->setRenderCallback(renderFrame);
    ffmpeg->prepare();

    env->ReleaseStringUTFChars(data_source, dataSource);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myndkapplication_Player_startNative(JNIEnv *env, jobject thiz) {
    if (ffmpeg) {
        ffmpeg->start();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myndkapplication_Player_setSurfaceNative(JNIEnv *env, jobject thiz,
                                                          jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = 0;
    }
    window = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mutex);
}