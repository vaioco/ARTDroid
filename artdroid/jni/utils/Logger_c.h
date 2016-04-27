//
// Created by vaioco on 22/04/16.
//

#ifndef ARTDROID_C_LOGGER_C_H
#define ARTDROID_C_LOGGER_C_H


#ifndef ALOG

#include <jni.h>
#include <android/log.h>

#define TAG "artdroid"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define ALOG(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG, __VA_ARGS__);

#endif

#endif //ARTDROID_C_LOGGER_C_H
