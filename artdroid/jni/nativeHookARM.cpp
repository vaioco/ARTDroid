//
// Created by vaioco on 19/05/16.
//


#include "jni.h"

extern void* my_newInstance(JNIEnv* e, jobject* obj);

void* my_newInstance_arm(JNIEnv* e, jobject* obj){
    return my_newInstance(e,obj);
}