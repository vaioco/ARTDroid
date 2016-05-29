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



//I need the offset of Constructor_newInstance
// it is 0x6af60 from JNI_GetCreatedJavaVMs_fnPtr for 5.1.1
//unsigned long int addr = p->JNI_GetCreatedJavaVMs_fnPtr + 0x6af60;
//crasha
//newInstance
/*
if(hook_address(&newinstanceh, getpid(), "libart.",
        "newInstance",
        my_newInstance_arm, my_newInstance, addr) == 0){
    ALOG("cannot find symbol newInstance!!\n");
    return;
}*/
// forname
/*
addr = p->JNI_GetCreatedJavaVMs_fnPtr + 0x651a4;
if(hook_address(&fornameh, getpid(), "libart.",
                "forName",
                my_forname_arm, my_forname, addr) == 0){
    ALOG("cannot find symbol forName!!\n");
    return;
}

//opendexfilenative
//0x5b8c4
addr = p->JNI_GetCreatedJavaVMs_fnPtr + 0x5b8c4;
if(hook_address(&opendexh, getpid(), "libart.",
                "openDexFileNative",
                my_openDexFileNative_arm, my_openDexFileNative, addr) == 0){
    ALOG("cannot find symbol openDexFileNative!!\n");
    return;
}
*/
/*
jobject curr_th = p->art_getCurrentThread_fnPtr(env, vmstack);
ALOG("current thread: %x \n", curr_th);
ALOG("current thread: %x \n", *curr_th);
jobjectArray st = 0x0;
//ALOG("got stacktrace1: %x \n", st);
st = p->art_getStackTrace_fnPtr(env, vmstack,  curr_th);
jsize len = env->GetArrayLength(st);
ALOG("got stacktrace len: %d \n", len);
int k = 0;
for(k=0;k<len;k++){
    jobject elem =  env->GetObjectArrayElement(st,k);
    ALOG("line num: %x \n", elem);
}
*/

/*
jobject curr_th = p->art_getCurrentThread_fnPtr(env, vmstack);
ALOG("current thread: %x \n", curr_th);
jobject stackT = p->art_getThreadStack_fnPtr(soa, curr_th);
jobjectArray test = p->art_InternalStackTraceToStackTraceElementArray_fnPtr(soa, stackT, nullptr, nullptr);
ALOG("jobjectArray result: %x \n", test);
len = env->GetArrayLength(test);
ALOG("got stacktrace len: %d \n", len);
*/

//wrapperToSeccon(env, javaMethod, javaReceiver, javaArgs);


/*
    static void wrapperToSeccon(JNIEnv* env, jobject method, jobject receiver, jobject args){
        if(hk_class != nullptr && dumpMethod != nullptr){
            ALOG("%s called!! \n", __PRETTY_FUNCTION__)
            //jclass mycls = _findClassFromDexCL(env, clsname);
            //jmethodID dumpMethod2 = (env)->GetStaticMethodID( hk_class, "dumpMethod2", "()V");
            //jobject gmeth = env->NewGlobalRef(method);
            //jobject grec = env->NewGlobalRef(receiver);
            //jobject gargs = env->NewGlobalRef(args);
            (env)->CallStaticVoidMethod(hk_class, dumpMethod, method, receiver, args);
            //(env)->DeleteGlobalRef(gmeth);
            //(env)->DeleteGlobalRef(grec);
            //(env)->DeleteGlobalRef(gargs);
            //(env)->CallStaticVoidMethod(mycls, dumpMethod2);
        }
    }
    static void wrapperToSecconForName(JNIEnv* env, jstring js, unsigned char uc, jobject obj){
        ALOG("%s called!! \n", __PRETTY_FUNCTION__)
        jobject gjs = env->NewGlobalRef(js);
        jobject gobj = env->NewGlobalRef(obj);
        (env)->CallStaticVoidMethod(hk_class, dumpForName, gjs, uc, gobj);
        (env)->DeleteGlobalRef(gjs);
        (env)->DeleteGlobalRef(gobj);
    }

    // art::Class_classForName(_JNIEnv*, _jclass*, _jstring*, unsigned char, _jobject*)

    void* my_forname(JNIEnv* e, jclass c,  jstring js, unsigned char uc, jobject obj)
    {
        ALOG("------- %s called. js: %x, char %x , obj = %x \n", __PRETTY_FUNCTION__, js, uc, obj);
        char* cc = e->GetStringUTFChars(js, NULL);
        ALOG("------ %s jstring: %s \n", __PRETTY_FUNCTION__, cc);
        //wrapperToSecconForName(e,js,uc,obj);
        void *(*orig_forName)(JNIEnv* e, jclass c,  jstring js, unsigned char uc, jobject obj);
        orig_forName = (void* ) fornameh.orig;
        hook_precall(&fornameh);
        void* res = orig_forName(e,c,js,uc,obj);
        hook_postcall(&fornameh);
        return res;
    }

    //art::Class_newInstance(_JNIEnv*, _jobject*)
    void* my_newInstance(JNIEnv* e, jobject obj){
        ALOG("+++++++ %s called with: %x , %x \n", __PRETTY_FUNCTION__, e, obj);
        void *(*orig_newInstance)(JNIEnv* e, jobject obj);
        orig_newInstance = (void* ) newinstanceh.orig;
        hook_precall(&newinstanceh);
        void* res = orig_newInstance(e,obj);
        hook_postcall(&newinstanceh);
        return res;

    }

    //art::DexFile_openDexFileNative(_JNIEnv*, _jclass*, _jstring*, _jstring*, int)
    //static jobject DexFile_openDexFileNative(
    //        JNIEnv* env, jclass, jstring javaSourceName, jstring javaOutputName, jint) {

    jobject my_openDexFileNative(JNIEnv* e,jclass c, jstring javaSourceName, jstring javaOutputName, int flags ){
        ALOG("+++++++ %s called with: %x , %d \n", __PRETTY_FUNCTION__, c, flags);
        char* cc = e->GetStringUTFChars(javaOutputName, NULL);
        ALOG("------ %s jstring: %s \n", __PRETTY_FUNCTION__, cc);
        void *(*orig_openDexFileNative)(JNIEnv* e, jclass c, jstring javaSourceName, jstring javaOutputName, int flags);
        orig_openDexFileNative = (void* ) opendexh.orig;
        hook_precall(&opendexh);
        jobject res = orig_openDexFileNative(e,c,javaSourceName,javaOutputName,flags);
        hook_postcall(&opendexh);
        return res;

    }
 */