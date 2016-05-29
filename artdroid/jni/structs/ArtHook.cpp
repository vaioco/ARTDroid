//
// Created by vaioco on 20/04/16.
//

#include "ArtHook.h"



namespace artdroid{

    void ArtHook::print(){
        string text ;
        text += "Hook: clsname: ";
        text += this->cname;
        text += " mname: ";
        text += this->mname;
        text += " hash: ";
        text += this->hash;
        ALOG("%s : %s\n", __PRETTY_FUNCTION__, text.c_str());
    }
    jboolean ArtHook::callOriginalMethodZ(jobject thiz, jobjectArray args) {
        ALOG("%s called with original: %x, thiz: %x \n", __PRETTY_FUNCTION__, this->original, thiz);
        jsize len;
        JNIEnv *env = getJNIEnv();
        jclass c = env->GetObjectClass(thiz);
        if (args) {
            len = env->GetArrayLength(args);
            ALOG("%s args len: %d \n", __PRETTY_FUNCTION__, len);
        }
        jvalue *_args = tryToUnbox(env, this, (unsigned int *) args, thiz, 0);
        jboolean _bool = env->CallNonvirtualBooleanMethodA(thiz, c, this->original, _args);
        ALOG("%s result: %x \n", __PRETTY_FUNCTION__, _bool);
        if (_bool) return JNI_TRUE;
        else return JNI_FALSE;
    }
    void* ArtHook::callOriginalMethod(jobject thiz, jobjectArray args) {
        ALOG("%s called with original: %x, thiz: %x \n", __PRETTY_FUNCTION__, this->original, thiz);
        jsize len;
        JNIEnv *env = getJNIEnv();
        jclass c = env->GetObjectClass(thiz);
        if (args){
            len = env->GetArrayLength(args);
            ALOG("%s args len: %d \n", __PRETTY_FUNCTION__, len);
        }
        jvalue* _args = tryToUnbox(env, this, (unsigned int*) args, thiz, 0);
        char* retype = calloc(1,sizeof(char));
        parseReturnType(env, this->msig.c_str(), retype );
        jobject _res;
        jboolean _bool;
        switch(*retype){
            case 'V':
                ALOG("%s return type is : %c \n", __PRETTY_FUNCTION__, *retype);
                env->CallNonvirtualVoidMethod(thiz, c, this->original, _args);
                break;
            case 'L':
                ALOG("%s return type is : %c, invoking original method: 0x%x \n", __PRETTY_FUNCTION__, *retype, this->original);
                _res =  env->CallNonvirtualObjectMethodA(thiz, c, this->original, _args);
                ALOG("%s result: %x \n", __PRETTY_FUNCTION__,_res);
                return env->NewGlobalRef(_res);
            default:
                break;
        }
    }
}