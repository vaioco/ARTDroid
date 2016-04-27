//
// Created by vaioco on 27/04/16.
//

#include "Hooking.h"

extern jboolean art_setup(JNIEnv* env);
extern void art_HookMethod(JNIEnv* env, artdroid::ArtHook& a);

namespace artdroid {
    extern void __attribute__ ((visibility ("hidden"))) setArtHook(ArtHook &a) {
        ALOG("%s called \n", __PRETTY_FUNCTION__);
        a.print();
        ALOG("%s setting hooks!! \n", __PRETTY_FUNCTION__);
        JNIEnv* env = getJNIEnv();
        art_setup(env);
        art_HookMethod(env, a);
    }
    jobject _callOriginalMethod2(JNIEnv* env, jobject obj, jstring key, jobject thiz,
                                        jobjectArray joa) {
        char *mykey = getCharFromJstring(env, key);
        ALOG("%s key = %s\n", __PRETTY_FUNCTION__, mykey);
        /*
        arthook_t *target = get_hook_by_key(mykey);
        if(!target) return NULL;
        ALOG("%s trovato target cname: %s mid: %x \n", __PRETTY_FUNCTION__, target->mname, target->original_meth_ID);
        jvalue* p = tryToUnbox(env, target, joa, thiz, false);
        jclass c = (*env)->GetObjectClass(env, thiz);
        jobject o = parseReturnType(env,target->msig,thiz,c,target->original_meth_ID, p);
        if(o != NULL)
            return (*env)->NewGlobalRef(env, o);
        //(*env)->CallNonvirtualVoidMethodA(env, thiz, c, target->original_meth_ID, p);
         */
        return nullptr;
    }


}