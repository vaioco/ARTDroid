//
// Created by vaioco on 21/04/16.
//

#ifndef ARTDROID_C_JNIHELPER_H
#define ARTDROID_C_JNIHELPER_H


#include "utils/Logger.h"
#include "utils/ARTStuff.h"
#include "utils/Misc.h"
#include "structs/Data.h"

#include <jni.h>
#include <string.h>
#include <unistd.h>

#undef NELEM
#define NELEM(x)            (sizeof(x)/sizeof(*(x)))


namespace artdroid {
    //static void _GetCreatedJavaVMs(struct artstuff_t &d, void **vms, jsize size, jsize *vm_count);

    static void _getJNIEnv(struct artstuff_t *dd,void** env, int version);
    extern "C" JNIEnv *getJNIEnv();

    static JavaVM* _vms = nullptr;
    static JNIEnv* _env = nullptr;
    static jobject _dexCL = nullptr;
    jobject _getSystemClassLoader();
    jobject _createDexClassLoader(jobject classLoader, string& mydexpath, string& myoptdir);
    jclass _loadClassFromClassLoader(jobject classLoader, string& targetName);
    jclass _loadClassFromCLAndRegisterNatives(jobject classLoader, string& targetName, bool flag);
    static void _GetCreatedJavaVMs(struct artstuff_t* dd, void **vms, jsize size, jsize *vm_count);
    jclass findClassFromClassLoader(JNIEnv* env, jobject classLoader, string& targetName);
    jclass _findClassFromDexCL(JNIEnv* env,string& targetName);
    int jniRegisterNativeMethods(JNIEnv* env, jclass cls);
    char* getCharFromJstring(JNIEnv* env, jstring jstr);
}


#endif //ARTDROID_C_JNIHELPER_H
