//
// Created by vaioco on 21/04/16.
//

#include "nativeHook.h"

#include <mutex>

//extern int hook(struct hook_t *h, int pid, char *libname, char *funcname, void *hook_arm, void *hook_thumb);

using namespace art;


namespace artdroid{


    extern void setArtHook(ArtHook&);

    extern void* my_newInstance_arm(JNIEnv *e, jobject obj);
    extern void* my_forname_arm(JNIEnv* e, jclass c,  jstring js, unsigned char uc, jobject obj);
    extern jobject my_openDexFileNative_arm(JNIEnv* e,jclass c, jstring javaSourceName, jstring javaOutputName, int flags );

    static int done = 0;
    static jclass hk_class = nullptr;
    static jclass vmstack = nullptr;
    static jmethodID dumpMethod = nullptr;
    static jmethodID dumpForName = nullptr;

    extern jobject gDexLoader;

    /*
     * new native reflection_invoke entrypoint
     */
    void* my_invoke_method(void* soa, jobject javaMethod, jobject javaReceiver, jobject javaArgs, unsigned char accessible) {
        ALOG("!!! %s receiver : 0x%08x, javamethod : 0x%08x \n", __PRETTY_FUNCTION__,
                    javaReceiver,  javaMethod);
        void *(*orig_invoke_method)(void *soa, void *javaMethod, void *javaReceiver, void *javaArgs, unsigned char accessible);
        void *res = NULL;
        orig_invoke_method = (void *) invokeh.orig;
        JNIEnv* env = getJNIEnv();
        /*
        art::mirror::ArtMethod* smeth =
                (art::mirror::ArtMethod*) env->FromReflectedMethod(javaMethod);
        art::mirror::Class* _declaringClass = (art::mirror::Class*) smeth->declaring_class_;
        ALOG("method invoked with reflection: 0x%x , class: 0x%x\n" , smeth, _declaringClass);
        */
        artstuff_t* p = &_art_wrapper;
        jclass _throw = env->FindClass("java/lang/Throwable");
        //TODO: multi-thread
        if(!done) {
            string dexpath = "/data/local/tmp/dex/target.dex";
            string optdir = "/data/local/tmp/dex/opt";
            string classname = "org/sid/arthookbridge/HookCls";
            jobject _dexloader = _createDexClassLoader(_getSystemClassLoader(), dexpath, optdir);
            gDexLoader = env->NewGlobalRef(_dexloader);
            hk_class = _loadClassFromCLAndRegisterNatives(_dexloader, classname, false);
            done = 1;
            DataStruct* c = DataStruct::getContainerInstance();
            jniRegisterNativeMethods(NULL, hk_class);
            c->setHooks( setArtHook );
            ALOG("Jar class loaded! \n");
        }
        hook_precall(&invokeh);
        res = orig_invoke_method(soa, javaMethod, javaReceiver, javaArgs, accessible);
        hook_postcall(&invokeh);
        return res;
    }

    /*
     * setting native hook via ADBI
     * previously target was epoll_wait() in libc
     */
    void setNativeHook(Config& c) {
    ALOG("%s, apiversion = %d \n", __PRETTY_FUNCTION__, c.apiversion);
    if(c.apiversion <= 19 ){
        if(hook(&invokeh, getpid(), "libart.",
                "_ZN3art12InvokeMethodERKNS_18ScopedObjectAccessEP8_jobjectS4_S4_",
                NULL, my_invoke_method) == 0){
            ALOG("cannot find symbol _ZN3art12InvokeMethodERKNS_18ScopedObjectAccessEP8_jobjectS4_S4_!!\n");
            return;
        }
    }
    else if(c.apiversion == 23)
    {
        if(hook(&invokeh, getpid(), "libart.",
                "_ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_j",
                NULL, my_invoke_method) == 0){
            ALOG("cannot find symbol _ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_j!!\n");
            return;
        }

    }
    else{
        //_ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_b
        if(hook(&invokeh, getpid(), "libart.",
                        "_ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_b",
                        NULL, my_invoke_method) == 0){
            ALOG("cannot find symbol _ZN3art12InvokeMethodERKNS_33ScopedObjectAccessAlreadyRunnableEP8_jobjectS4_S4_b!!\n");
            return;
        }

    }
}
}


/*
 *
 * STADYNA
 *
 *
 */
