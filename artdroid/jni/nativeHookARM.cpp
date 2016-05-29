//
// Created by vaioco on 19/05/16.
//


#include "jni.h"

namespace artdroid {
    extern void *my_newInstance(JNIEnv *e, jobject obj);
    extern void* my_forname(JNIEnv* e, jclass c,  jstring js, unsigned char uc, jobject obj);
    extern jobject my_openDexFileNative(JNIEnv* e,jclass c, jstring javaSourceName, jstring javaOutputName, int flags );

    jobject my_openDexFileNative_arm(JNIEnv* e,jclass c, jstring javaSourceName, jstring javaOutputName, int flags )
    {
        return my_openDexFileNative(e, c,  javaSourceName,  javaOutputName,  flags );
    }
    void *my_newInstance_arm(JNIEnv *e, jobject obj) {
        return my_newInstance(e, obj);
    }

    void* my_forname_arm(JNIEnv* e, jclass c,  jstring js, unsigned char uc, jobject obj)
    {
        return my_forname(e,c,js,uc,obj);
    }

}