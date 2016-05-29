//
// Created by vaioco on 21/04/16.
//

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <jni.h>

#include "utils/Logger_c.h"

#ifndef ARTDROID_C_ARTSTUFF_H
#define ARTDROID_C_ARTSTUFF_H

//namespace artdroid {

    typedef jint (*JNI_GetCreatedJavaVMs_func)(void **, jsize, jsize *);

    typedef void *(*art_th_currentFromGdb_func)();

    typedef void (*art_dbg_SuspendVM_func)();

    typedef void (*art_dbg_SuspendSelf_func)();

    typedef void (*art_thlist_resumeAll_func)();

    typedef void (*art_dbg_ResumeVM_func)();

    typedef void (*art_dumpallclasses_func)(int);

    typedef jint (*art_getenv_func)(JavaVM*, void**, int);

    typedef jint (*art_checkgetenv_func)(JavaVM*, void**, int);

    typedef void (*art_printloadedclasses_func)(int);

    typedef jobjectArray (*art_getStackTrace_func)(JNIEnv*, jclass, jobject);

    typedef jobject (*art_getCurrentThread_func)(JNIEnv*, jclass);

    typedef jobject (*art_getThreadStack_func)(void*, jobject);
    //_ZN3artL32Throwable_nativeFillInStackTraceEP7_JNIEnvP7_jclass
    typedef jobjectArray (*art_nativeFillInStackTrace_func)(JNIEnv*, jclass);
    //_ZN3artL29Throwable_nativeGetStackTraceEP7_JNIEnvP7_jclassP8_jobject
    typedef jobjectArray (*art_nativeGetStackTrace_func)(JNIEnv*, jclass, jobject);
    //_ZN3art11ClassLinker27AllocStackTraceElementArrayEPNS_6ThreadEj
    typedef jobjectArray (*art_AllocStackTraceElementArray_func)(void* , unsigned int);
    typedef jobjectArray (*art_InternalStackTraceToStackTraceElementArray_func)(void*, jobject, jobjectArray, int*);

    typedef struct artstuff_t {
        void *art_hand;
        void *art_runtime_instance;
        JNI_GetCreatedJavaVMs_func JNI_GetCreatedJavaVMs_fnPtr;
        art_th_currentFromGdb_func art_th_currentFromGdb_fnPtr;
        art_dbg_SuspendVM_func art_dbg_SuspendVM_fnPtr;
        art_dbg_SuspendSelf_func art_dbg_SuspendSelf_fnPtr;
        art_thlist_resumeAll_func art_thlist_resumeAll_fnPtr;
        art_dbg_ResumeVM_func art_dbg_ResumeVM_fnPtr;
        art_dumpallclasses_func art_dumpallclasses_fnPtr;
        art_printloadedclasses_func art_printloadedclasses_fnPtr;
        art_getenv_func art_getenv_fnPtr;
        art_checkgetenv_func art_checkgetenv_fnPtr;
        art_getStackTrace_func art_getStackTrace_fnPtr;
        art_getCurrentThread_func art_getCurrentThread_fnPtr;
        art_getThreadStack_func art_getThreadStack_fnPtr;
        art_InternalStackTraceToStackTraceElementArray_func art_InternalStackTraceToStackTraceElementArray_fnPtr;
        art_nativeGetStackTrace_func art_nativeGetStackTrace_fnPtr;
        art_nativeFillInStackTrace_func art_nativeFillInStackTrace_fnPtr;
        art_AllocStackTraceElementArray_func art_AllocStackTraceElementArray_fnPtr;
    }artstuff_t;

    extern artstuff_t _art_wrapper;

    int resolveSymbols(artstuff_t* );


//}
#endif


#ifdef __cplusplus
}
#endif