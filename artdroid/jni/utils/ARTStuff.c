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

