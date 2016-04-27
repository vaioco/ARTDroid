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

    typedef void (*art_printloadedclasses_func)(int);
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
    }artstuff_t;
    extern artstuff_t _art_wrapper;

    int resolveSymbols(artstuff_t* );


//}
#endif


#ifdef __cplusplus
}
#endif