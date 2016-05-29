//
// Created by vaioco on 21/04/16.
//

#include "JNIHelper.h"

namespace artdroid  {

    //return the JNIEnv* for the current thread
    extern "C" JNIEnv* getJNIEnv()
    {
        ALOG("%s called!!\n", __PRETTY_FUNCTION__);
        /*
        if (_env != nullptr) {
            ALOG("%s returning: %x \n", __PRETTY_FUNCTION__, _env);
            return _env;
        }
        */
        ALOG("_env null, getting JNIEnv*");
        jsize vm_count = 0;
        jsize size = 1;
        if (_vms == nullptr) {
            ALOG("%s ERROR vms null!! \n", __PRETTY_FUNCTION__);
            _GetCreatedJavaVMs(&_art_wrapper, (void**)&_vms, size, &vm_count);
        }
        JNIEnv* my_env = nullptr;
        _getJNIEnv(&_art_wrapper, &my_env, JNI_VERSION_1_6);
        //_env = (JNIEnv*) tmpenv->NewGlobalRef((jobject) tmpenv);
        ALOG("jnienv = 0x%08x \n", (unsigned int) my_env);
        return my_env;
    }
    //return the system class loader
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
    /*
     * create the dex loader for the "mydexpath" dex file
     * storing the output in myoptdir
     */
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
    // load the targetName class using the classLoader object
    jclass _loadClassFromClassLoader(jobject classLoader, string& targetName)
    {
        jclass test = NULL;
        JNIEnv* env = getJNIEnv();
        ALOG(" %s, targetname = %s \n ",__PRETTY_FUNCTION__, targetName.c_str());
        jclass classLoader_cls = env->FindClass("java/lang/ClassLoader");
        jmethodID loadClass = env->GetMethodID(classLoader_cls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        ALOG("loadClass mid = %p \n", loadClass);
        jstring name = env->NewStringUTF(targetName.c_str());
        ALOG("searching class: %s \n", targetName.c_str() );
        jclass loaded = env->CallObjectMethod(classLoader, loadClass, name);
        ALOG("loaded class = %p \n" , loaded);
        if(!loaded){
            ALOG("error retriving class: %s \n", targetName.c_str() );
            return -1;
        }
        jclass globalref = (jclass) env->NewGlobalRef(loaded);
        return (jclass) globalref;
    }
    // wrapper method to load and register native methods
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
    // return the JavaVM calling the function exported by libart
    // JavaVM, in contranst to JNIEnv*, is unique for each app
    static void _GetCreatedJavaVMs(struct artstuff_t* dd, void **vms, jsize size, jsize *vm_count) {
        ALOG("dentro getcreatedjavavms, vm_count : %d \n", *vm_count);
        jint res = 3;
        ALOG("chiamo: 0x%08x \n ", (unsigned int) dd->JNI_GetCreatedJavaVMs_fnPtr);
        res = dd->JNI_GetCreatedJavaVMs_fnPtr(vms, size, vm_count);
        ALOG("res = %d\n", res);
        if (res != JNI_OK) {
            ALOG("error!!!!\n");
        }
        ALOG("vm_count : %d \n", *vm_count);
    }
    // wrapper to getJNIEnv
    static void _getJNIEnv(struct artstuff_t *dd, void** env, int version) {
        ALOG("%s called using : %x !! \n", __PRETTY_FUNCTION__, _vms);
        int status = _vms->GetEnv(env, version);
        if(status < 0) {
            status = _vms->AttachCurrentThread(env, NULL);
            if(status != JNI_OK ) {
                ALOG("%s error getting JNIEnv!!!\n", __PRETTY_FUNCTION__);
                return NULL;
            }
        }
        /*
        ALOG("%s called using : %x !! \n", __PRETTY_FUNCTION__, vm_p);
        //jint res = dd->art_getenv_fnPtr(vm_p, env, version);
        //ALOG("result: %d \n", res);
        if(vm_p->GetEnv(reinterpret_cast<void**>(&env), version) != JNI_OK)
        {
            ALOG("%s error getting JNIEnv!!!\n", __PRETTY_FUNCTION__);
            return NULL;
        }
        jint status = vm_p->AttachCurrentThread( (void**) &env, NULL);
        if (status < 0) {
            ALOG("%s error getting JNIEnv!!!\n", __PRETTY_FUNCTION__);
            return NULL;
        }*/
    }
    //getter static object
    //TODO: add data struct
    static jobject getDexCL()
    {
        return _dexCL;
    }
    // convert a jstring to char*
    char* getCharFromJstring(JNIEnv* env, jstring jstr)
    {
        jsize len = (env)->GetStringUTFLength( jstr);
        char* res = (char*) calloc(len+1,1);
        (env)->GetStringUTFRegion( jstr, 0, len, res);
        //(*env)->DeleteLocalRef(env, jstr);
        ALOG("getcharfromj, res: %s \n", res);
        return res;
    }
    // find the targetName class in classLoader
    jclass findClassFromClassLoader(JNIEnv* env, jobject classLoader, string& targetName)
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
    // find the targetName class in the dexClassloader
    jclass _findClassFromDexCL(JNIEnv* env,string& targetName)
    {
        return findClassFromClassLoader(env, _dexCL, targetName);
    }
    // callback used to call the original method from the patch code
    static void* _callOriginalMethod(JNIEnv* env, jclass c, jstring key,
                                     jobject thiz, jobjectArray args)
    {
        ALOG("%s called \n", __PRETTY_FUNCTION__);
        string _key = ConvertJString(env, key);
        ALOG("%s searching for key: %s \n", __PRETTY_FUNCTION__, _key.c_str() );
        DataStruct *d = DataStruct::getContainerInstance();
        ArtHook* hook = d->search(_key);
        ALOG("found hook with name: %s \n", hook->cname.c_str());
        return hook->callOriginalMethod(thiz, args);

    }
    // wrapper to call original method returning a jboolean
    static jboolean _callOriginalMethodZ(JNIEnv* env, jclass c, jstring key,
                                     jobject thiz, jobjectArray args)
    {
        ALOG("%s called \n", __PRETTY_FUNCTION__);
        string _key = ConvertJString(env, key);
        ALOG("%s searching for key: %s \n", __PRETTY_FUNCTION__, _key.c_str() );
        DataStruct *d = DataStruct::getContainerInstance();
        ArtHook* hook = d->search(_key);
        ALOG("found hook with name: %s \n", hook->cname.c_str());
        return hook->callOriginalMethodZ(thiz, args);

    }
    // array of native functions exposed to java side
    static JNINativeMethod artHookMethods[] = {
            /* name, signature, funcPtr */
            {"callOriginalMethod",
                    "(Ljava/lang/String;Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;",
                    (void*) _callOriginalMethod },
            {"callOriginalMethodBoolean",
                    "(Ljava/lang/String;Ljava/lang/Object;[Ljava/lang/Object;)Z",
                    (void*) _callOriginalMethodZ }
    };
    // register exposed native functions
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