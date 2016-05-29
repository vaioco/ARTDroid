/*
 *
 * Copyright (c) 2015, alipay.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * 	art_method_replace.cpp
 *
 * @author : sanping.li@alipay.com
 *
 */
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include <stdbool.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include "art.h"

static int apilevel = -1;

static void HookInMem(artdroid::ArtHook& t, JNIEnv* env);

extern jboolean __attribute__ ((visibility ("hidden"))) art_setup(JNIEnv* env) {
    if(apilevel != -1) return JNI_TRUE;
    ALOG("%s called \n", __PRETTY_FUNCTION__);
    artdroid::Config* c = artdroid::Config::getConfigInstance();
	apilevel = c->apiversion;
	return JNI_TRUE;
}

extern void __attribute__ ((visibility ("hidden"))) art_HookMethod(
		JNIEnv* env, artdroid::ArtHook& target) {
	if (apilevel > 22) {
		//TODO: add marshmallow
		;//replace_6_0(env, src, dest);
	} else if (apilevel > 21) {
		ALOG("%s hooking on %s : %s \n", __PRETTY_FUNCTION__, target.mname.c_str(), target.msig.c_str());
        HookInMem(target, env);
	} else {
		;//replace_5_0(env, src, dest);
	}
}

static void HookInMem(artdroid::ArtHook& t, JNIEnv* env){
    ALOG("%s searching class: %s\n", __PRETTY_FUNCTION__, t.cname.c_str());
    test_5_1(env, t);
    }

/*
extern void __attribute__ ((visibility ("hidden"))) art_setFieldFlag(
		JNIEnv* env, jobject field) {
	if (apilevel > 22) {
		setFieldFlag_6_0(env, field);
	} else if (apilevel > 21) {
		setFieldFlag_5_1(env, field);
	} else {
		setFieldFlag_5_0(env, field);
	}
}
 */

