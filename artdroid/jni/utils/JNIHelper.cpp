//
// Created by vaioco on 21/04/16.
//

#include "JNIHelper.h"

namespace artdroid {

    extern "C" JNIEnv *getJNIEnv()
    {
        ALOG("%s called!!\n", __PRETTY_FUNCTION__);
        if (_env != nullptr) {
            ALOG("%s returning: %x \n", __PRETTY_FUNCTION__, _env);
            return _env;
        }
        jsize vm_count = 0;
        jsize size = 0;
        if (_vms == nullptr) {
            ALOG("%s ERROR vms null!! \n", __PRETTY_FUNCTION__);
            _GetCreatedJavaVMs(&_art_wrapper, (void**) &_vms, size, &vm_count);
        }
        ALOG("called getcreatedjavavms, results: %p , count = %d \n", _vms[0], vm_count);
        _env = _getJNIEnv();
        ALOG("jnienv = 0x%08x \n", (unsigned int) _env);
        return _env;
    }
    static jobject _getSystemClassLoader()
    {
        JNIEnv* env = getJNIEnv();
        ALOG("%s CALLED with: %x \n", __PRETTY_FUNCTION__, env);
        jclass classLoader = env->FindClass("java/lang/ClassLoader");
        ALOG("classloader class =  %p \n ", classLoader);
        jmethodID getSystemCL = env->GetStaticMethodID( classLoader, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
        jobject systemCL = env->CallStaticObjectMethod(classLoader, getSystemCL);
        ALOG("systemclassloader = %p \n ", systemCL);
        return systemCL;
    }
    jobject _createDexClassLoader(jobject classLoader, string& mydexpath, string& myoptdir)
    {
        jthrowable exc;
        JNIEnv* env = getJNIEnv();
        ALOG("%s , pid: %d\n",__PRETTY_FUNCTION__, getpid());
        jclass dexclassloader_cls = env->FindClass("dalvik/system/DexClassLoader");
        ALOG("dexclassloader class = %p \n", dexclassloader_cls);
        jmethodID constructor = env->GetMethodID(dexclassloader_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
        ALOG("init methodid = %p \n", constructor);
        // il proprietario di questa dir deve essere l'app
        jstring dexPathJ = env->NewStringUTF(mydexpath.c_str());
        jstring optDirJ = env->NewStringUTF(myoptdir.c_str());
        jobject dexloader = env->NewObject(dexclassloader_cls, constructor, dexPathJ, optDirJ, NULL, classLoader);
        exc = env->ExceptionOccurred();
        if(exc){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        _dexCL = (env)->NewGlobalRef(dexloader);
        ALOG("created dex loader = %p \n ", _dexCL);
        return _dexCL;
    }
    jclass _loadClassFromClassLoader(jobject classLoader, string& targetName)
    {
        jclass test = NULL;
        JNIEnv* env = getJNIEnv();
        //test = (jclass) check_cache(targetName);
        //if(test != 0) return test;
        ALOG(" %s, targetname = %s \n ",__PRETTY_FUNCTION__, targetName.c_str());
        jclass classLoader_cls = env->FindClass("java/lang/ClassLoader");
        jmethodID loadClass = env->GetMethodID(classLoader_cls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        ALOG("loadClass mid = %p \n", loadClass);
        jstring name = env->NewStringUTF(targetName.c_str());
        jclass loaded = env->CallObjectMethod(classLoader, loadClass, name);
        ALOG("loaded class = %p \n" , loaded);
        jclass globalref = (jclass) env->NewGlobalRef(loaded);
        //create_cache(targetName, globalref);
        return (jclass) globalref;
    }
    jclass _loadClassFromCLAndRegisterNatives(jobject classLoader, string& targetName, bool flag)
    {
        ALOG("%s called\n", __PRETTY_FUNCTION__);
        jclass c = _loadClassFromClassLoader(classLoader, targetName);
        if(flag){
            JNIEnv* env = getJNIEnv();
            jniRegisterNativeMethods(env, c);
        }
        return c;
    }
    static void _GetCreatedJavaVMs(struct artstuff_t* dd, void **vms, jsize size, jsize *vm_count) {
        ALOG("dentro getcreatedjavavms \n");
        jint res = 3;
        ALOG("chiamo: 0x%08x \n ", (unsigned int) dd->JNI_GetCreatedJavaVMs_fnPtr);
        res = dd->JNI_GetCreatedJavaVMs_fnPtr(vms, size, vm_count);
        ALOG("res = %d\n", res);
        if (res != JNI_OK) {
            ALOG("error!!!!\n");
        }
    }
    static JNIEnv *_getJNIEnv() {
        JNIEnv *env;
        int status = _vms[0].GetEnv((void **) &env, JNI_VERSION_1_6);
        if (status < 0) {
            status = _vms[0].AttachCurrentThread(&env, NULL);
            if (status < 0) {
                return NULL;
            }
        }
        return env;
    }
    static jobject getDexCL()
    {
        return _dexCL;
    }
    char* getCharFromJstring(JNIEnv* env, jstring jstr)
    {
        jsize len = (env)->GetStringUTFLength( jstr);
        char* res = (char*) calloc(len+1,1);
        (env)->GetStringUTFRegion( jstr, 0, len, res);
        //(*env)->DeleteLocalRef(env, jstr);
        ALOG("getcharfromj, res: %s \n", res);
        return res;
    }
    static jclass _findClassFromClassLoader(JNIEnv* env, jobject classLoader, string& targetName)
    {
        ALOG("%s called with targetname = %s \n", __PRETTY_FUNCTION__, targetName.c_str());
        jclass classLoader_cls = (env)->FindClass("java/lang/ClassLoader");
        jmethodID findClass = (env)->GetMethodID( classLoader_cls, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        ALOG("findClass mid = %p \n", findClass);
        jstring name = (env)->NewStringUTF(targetName.c_str());
        jclass res = (jclass) (env)->CallObjectMethod(classLoader,findClass,name);
        ALOG("HookClass =  %p\n" , res);
        jclass globalref = (jclass) (env)->NewGlobalRef(res);
        return (jclass) globalref;
    }
    jclass _findClassFromDexCL(JNIEnv* env,string& targetName)
    {
        _findClassFromClassLoader(env, _dexCL, targetName);
    }
    static JNINativeMethod artHookMethods[] = {
            /* name, signature, funcPtr */
            {"callOriginalMethod2",
                    "(Ljava/lang/String;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;",
                    (void*) _callOriginalMethod2 },
    };
    int jniRegisterNativeMethods(JNIEnv* env, jclass cls)
    {
        if(env == NULL)
            env = getJNIEnv();
        ALOG("%s called! \n", __PRETTY_FUNCTION__);
        if ((env)->RegisterNatives(cls, artHookMethods, NELEM(artHookMethods)) < 0) {
            return 1;
        }
        ALOG("%s native methods registered!! \n", __PRETTY_FUNCTION__);
        return 0;
    }
}