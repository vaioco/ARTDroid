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
#include "structs/art/art_5_1.h"
#include "utils/ARTStuff.h"
#include "Globals.h"
#include "Hooking.h"

extern "C" {
#include "hook.h"
}

namespace artdroid {
    static struct hook_t invokeh;
    static struct hook_t newinstanceh;
    static struct hook_t fornameh;
    static struct hook_t opendexh;

    void* my_invoke_method(void* soa, jobject javaMethod, jobject javaReceiver, jobject javaArgs, unsigned char flag);
    void setNativeHooks(Config& c);
}


#endif //ARTDROID_C_NATIVEHOOK_H
