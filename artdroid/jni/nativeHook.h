//
// Created by vaioco on 21/04/16.
//

#ifndef ARTDROID_C_NATIVEHOOK_H
#define ARTDROID_C_NATIVEHOOK_H

#include <jni.h>
#include <unistd.h>

#include "utils/Logger.h"
#include "conf/AParser.h"
#include "utils/JNIHelper.h"
#include "structs/Data.h"
#include "structs/ArtHook.h"
#include "Globals.h"
#include "Hooking.h"

extern "C" {
#include "hook.h"
}

namespace artdroid {
    static struct hook_t invokeh;
    void* my_invoke_method(void* soa, jobject javaMethod, void* javaReceiver, jobject javaArgs);
    void setNativeHooks(Config& c);
}


#endif //ARTDROID_C_NATIVEHOOK_H
