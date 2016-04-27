//
// Created by vaioco on 22/04/16.
//

#ifndef ARTDROID_C_MYDEXLOADER_H
#define ARTDROID_C_MYDEXLOADER_H

#include <jni.h>
#include "utils/Logger.h"
#include "utils/JNIHelper.h"


namespace artdroid {
    class MyDexLoader {
    public:
        static jobject getSystemClassLoader(JNIEnv *e);

        static jobject createDexClassLoader(JNIEnv *e, jobject sysCL, string &dexfile, string &optdir);

        static jclass loadClassFromClassLoader(JNIEnv *e, jobject dexCL, string &clsname);

        static jobject set_dexloader(JNIEnv *env, string &dexfile);
        };
}


#endif //ARTDROID_C_MYDEXLOADER_H
