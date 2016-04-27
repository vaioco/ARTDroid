//
// Created by vaioco on 27/04/16.
//

#ifndef ARTDROID_C_HOOKING_H
#define ARTDROID_C_HOOKING_H

#include "structs/ArtHook.h"
#include "utils/Logger.h"
#include "conf/AParser.h"
#include "structs/art/art.h"
#include "utils/JNIHelper.h"

using namespace std;

namespace artdroid {
        extern void setArtHook(ArtHook& a);
        jobject _callOriginalMethod2(JNIEnv* env, jobject obj, jstring key, jobject thiz,
                                 jobjectArray joa);
}


#endif //ARTDROID_C_HOOKING_H
