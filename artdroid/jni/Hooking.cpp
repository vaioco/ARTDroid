//
// Created by vaioco on 27/04/16.
//

#include "Hooking.h"

extern jboolean art_setup(JNIEnv* env);
extern void art_HookMethod(JNIEnv* env, artdroid::ArtHook& a);

namespace artdroid {
    /*
     * callback
     * Call arthook_helper to search and hijack the target method contained in target
     */
    extern void __attribute__ ((visibility ("hidden"))) setArtHook(ArtHook &target) {
        ALOG("%s called \n", __PRETTY_FUNCTION__);
        target.print();
        ALOG("%s setting hooks!! \n", __PRETTY_FUNCTION__);
        JNIEnv* env = getJNIEnv();
        art_setup(env);
        art_HookMethod(env, target);
    }
}