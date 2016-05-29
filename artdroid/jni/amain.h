//
// Created by vaioco on 21/03/16.
//
#ifndef ARTDROID_C_ARTDROIDMAIN_H
#define ARTDROID_C_ARTDROIDMAIN_H

#include <jni.h>
#include <string>

#include "utils/Logger.h"
#include "conf/AParser.h"
#include "structs/Data.h"
#include "utils/MyDexLoader.h"
#include "utils/JNIHelper.h"
#include "utils/ARTStuff.h"
#include "Globals.h"

#include "nativeHook.h"

extern "C"{
#include "base.h"
}

using namespace std;

extern "C" void artlogmsgtofile(char* msg);
extern "C" void *set_logfunction(void *func);

namespace artdroid {
    void mylog2(char *msg);
    extern "C" int resolveSymbols(artstuff_t* );

    int artdroid_main( string,  string);
}

#endif //ARTDROID_C_ARTDROIDMAIN_H
