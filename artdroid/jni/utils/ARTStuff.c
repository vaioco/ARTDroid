//
// Created by vaioco on 21/04/16.
//

#include "ARTStuff.h"


//namespace artdroid{
    artstuff_t _art_wrapper;

    static void* mydlsym(void *hand, const char *name)
    {
        void* ret = dlsym(hand, name);
        ALOG("%s = %p\n", name, ret);
        return ret;
    }
static void _GetCreatedJavaVMs(artstuff_t* d, void** vms, jsize size, jsize* vm_count){
    ALOG("dentro getcreatedjavavms \n");
    jint res = 3;
    ALOG("chiamo: 0x%08x \n ", (unsigned int) d->JNI_GetCreatedJavaVMs_fnPtr);
    res = d->JNI_GetCreatedJavaVMs_fnPtr(vms, size, vm_count);
    ALOG("res = %d\n", res);
    if(res != JNI_OK){
        ALOG("error!!!!\n");
    }
}

int resolveSymbols(artstuff_t* d){
    d->art_hand = dlopen("libart.so", RTLD_NOW);
    ALOG("art_hand = 0x%08x \n", (unsigned int) d->art_hand);
    if(d->art_hand != NULL){
        d->JNI_GetCreatedJavaVMs_fnPtr = mydlsym(d->art_hand, "JNI_GetCreatedJavaVMs");
        //_ZN3art3JII6GetEnvEP7_JavaVMPPvi
        d->art_getenv_fnPtr = d->JNI_GetCreatedJavaVMs_fnPtr + 0x39368;
        ALOG("%s = 0x%0x \n", "_ZN3art3JII6GetEnvEP7_JavaVMPPvi",  d->art_getenv_fnPtr);
        //0x64c14 _ZN3artL27VMStack_getThreadStackTraceEP7_JNIEnvP7_jclassP8_jobject
        d->art_getStackTrace_fnPtr = d->JNI_GetCreatedJavaVMs_fnPtr + 0x64c14;
        ALOG("%s = 0x%0x \n", " _ZN3artL27VMStack_getThreadStackTraceEP7_JNIEnvP7_jclassP8_jobject",
             d->art_getStackTrace_fnPtr);
        //0x676c0 _ZN3artL20Thread_currentThreadEP7_JNIEnvP7_jclass
        d->art_getCurrentThread_fnPtr = d->JNI_GetCreatedJavaVMs_fnPtr + 0x676c0;
        ALOG("%s = 0x%0x \n", " _ZN3artL20Thread_currentThreadEP7_JNIEnvP7_jclass",
             d->art_getCurrentThread_fnPtr);

        d->art_getThreadStack_fnPtr = d->JNI_GetCreatedJavaVMs_fnPtr + 0x63f1c;
        ALOG("%s = 0x%0x \n", " _ZN3artL14GetThreadStackERKNS_28ScopedFastNativeObjectAccessEP8_jobject",
             d->art_getThreadStack_fnPtr);

        //_ZN3art6Thread42InternalStackTraceToStackTraceElementArrayERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectP13_jobjectArrayPi
        d->art_InternalStackTraceToStackTraceElementArray_fnPtr =
                mydlsym(d->art_hand, "_ZN3art6Thread42InternalStackTraceToStackTraceElementArrayERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectP13_jobjectArrayPi");
        d->art_nativeGetStackTrace_fnPtr = mydlsym(d->art_hand,
                                                   "_ZN3artL29Throwable_nativeGetStackTraceEP7_JNIEnvP7_jclassP8_jobject");
        d->art_nativeFillInStackTrace_fnPtr = mydlsym(d->art_hand,
                                                    "_ZN3artL32Throwable_nativeFillInStackTraceEP7_JNIEnvP7_jclass");
        d->art_AllocStackTraceElementArray_fnPtr = mydlsym(d->art_hand,
                                                    "_ZN3art11ClassLinker27AllocStackTraceElementArrayEPNS_6ThreadEj");
        d->art_th_currentFromGdb_fnPtr = mydlsym(d->art_hand, "_ZN3art6Thread14CurrentFromGdbEv");
        d->art_dbg_SuspendVM_fnPtr = mydlsym(d->art_hand, "_ZN3art3Dbg9SuspendVMEv");
        d->art_dbg_ResumeVM_fnPtr = mydlsym(d->art_hand, "_ZN3art3Dbg8ResumeVMEv");
        d->art_dbg_SuspendSelf_fnPtr = mydlsym(d->art_hand, "_ZN3art3Dbg11SuspendSelfEv");
        d->art_thlist_resumeAll_fnPtr = mydlsym(d->art_hand, "_ZN3art10ThreadList9ResumeAllEv");
        d->art_runtime_instance = mydlsym(d->art_hand,"_ZN3art7Runtime9instance_E");
        d->art_dumpallclasses_fnPtr = mydlsym(d->art_hand,"_ZN3art11ClassLinker14DumpAllClassesEi");
        //d->art_printloadedclasses_fnPtr = mydlsym(d->art_hand,"_ZN3artL26VMDebug_printLoadedClassesEP7_JNIEnvP7_jclassi");
    }
    else {
        ALOG("cannot dlopen libart.so");
        ALOG("ERROR are you using the ART runtime? \n");
        return 1;
    }
    return 0;
}

//}

