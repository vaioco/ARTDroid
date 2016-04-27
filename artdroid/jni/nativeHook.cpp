//
// Created by vaioco on 21/04/16.
//

#include "nativeHook.h"

//extern int hook(struct hook_t *h, int pid, char *libname, char *funcname, void *hook_arm, void *hook_thumb);

namespace artdroid{

    extern void setArtHook(ArtHook&);


    void* my_invoke_method(void* soa, jobject javaMethod, void* javaReceiver, jobject javaArgs) {
        ALOG("!!! %s receiver : 0x%08x, javamethod : 0x%08x \n", __PRETTY_FUNCTION__,
                   (unsigned int) javaReceiver, (unsigned int) javaMethod);
        void *(*orig_invoke_method)(void *soa, void *javaMethod, void *javaReceiver, void *javaArgs);
        void *res = NULL;
        orig_invoke_method = (void *) invokeh.orig;
        //CODE
        string dexpath = "/data/local/tmp/dex/target.dex";
        string optdir = "/data/local/tmp/dex/opt";
        string classname = "org/sid/arthookbridge/HookCls";



        jobject dexloader = _createDexClassLoader(_getSystemClassLoader(), dexpath, optdir);
        jclass hkclass = _loadClassFromCLAndRegisterNatives(dexloader, classname, false);
        DataStruct* c = DataStruct::getContainerInstance();
        jniRegisterNativeMethods(NULL, hkclass);
        c->setHooks( setArtHook );



        hook_precall(&invokeh);
        res = orig_invoke_method(soa, javaMethod, javaReceiver, javaArgs);
        //hook_postcall(&invokeh);
        return res;
    }
    void setNativeHooks(Config& c) {
        ALOG("%s, apiversion = %d \n", __PRETTY_FUNCTION__, c.apiversion);
        if(c.apiversion <= 19 ){
            if(hook(&invokeh, getpid(), "libart.",
                    "_ZN3art12InvokeMethodERKNS_18ScopedObjectAccessEP8_jobjectS4_S4_",
                    NULL, my_invoke_method) == 0){
                ALOG("cannot find symbol _ZN3art12InvokeMethodERKNS_18ScopedObjectAccessEP8_jobjectS4_S4_!!\n");
                return;
            }
        }
        else{
            if(hook(&invokeh, getpid(), "libart.",
                    "_ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_b",
                    NULL, my_invoke_method) == 0){
                ALOG("cannot find symbol _ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_b!!\n");
                return;
            }
        }
    }
}