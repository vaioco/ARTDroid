//
// Created by vaioco on 19/04/16.
//


#ifndef ARTDROID_C_LOGGER_H
#define ARTDROID_C_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif


#include <jni.h>
#include <android/log.h>

#define TAG "artdroid"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define ALOG(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG, __VA_ARGS__);


#ifdef __cplusplus
}
#endif

#include <string>
#include <cstring>

using namespace std;

namespace artdroid {
    class Logger {
    public:
        string log_file;
        Logger(const string& fname) : log_file{fname}{};
    };
}



#endif //ARTDROID_C_LOGGER_H
