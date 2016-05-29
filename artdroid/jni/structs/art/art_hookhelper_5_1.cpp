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
 * 	art_method_replace_5_1.cpp
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

#include "art.h"
#include "art_5_1.h"
#include "utils/JNIHelper.h"

bool test_5_1(JNIEnv* env, artdroid::ArtHook& t){
    ALOG("%s mname = %s , signature: %s \n", __PRETTY_FUNCTION__, t.mname.c_str(), t.msig.c_str() );
    /*
     * get the target class reference
     */
    jclass c = env->FindClass(t.cname.c_str());
    /*
     * get the target method reference
     */
    void* mid = env->GetMethodID(c, t.mname.c_str(), t.msig.c_str() );
    /*
     * store the reference in the "t" ArtHook's field
     */
	memcpy(t.original,mid, sizeof(mid));
    /*
     * Find the patch-code class
     */
    jclass c2 = artdroid::_findClassFromDexCL(env, t.hkname);
    /*
     * get and store the patch-code method
     */
    unsigned int* _pc = (unsigned int *) (env)->GetMethodID(c2, t.mname.c_str(), t.msig.c_str());
    t.patch = *_pc;
    // cast the above values to ArtMethod*
    art::mirror::ArtMethod* patchMethod = (art::mirror::ArtMethod*) _pc;
	art::mirror::ArtMethod* originalMethod = (art::mirror::ArtMethod*) mid;
    // get methods' index in the vtable
    uint32_t mid_index = originalMethod->method_index_;
    uint32_t mid_index2 = patchMethod->method_index_;

    /*
    ALOG("%s swapping declaring class: ", __PRETTY_FUNCTION__);
    ALOG("0x%x <- 0x%x \n", patchMethod->declaring_class_ , originalMethod->declaring_class_);
    *(unsigned int*)patchMethod->declaring_class_ = (unsigned int*) originalMethod->declaring_class_;
    */

    art::mirror::Class* dec_class = (art::mirror::Class*) originalMethod->declaring_class_;
    /*
     * get pointer to vtable stored in dec_class
     */
    unsigned int* _vmeth = dec_class->virtual_methods_;
    /*
     * get the pointer to the target method stored in vtable
     */
    unsigned int* _meth = ( (unsigned int) originalMethod->declaring_class_ + mid_index * 4 + 0x170 ) ;

    ALOG("%s BEFORE vmeth value : 0x%x -> 0x%x , meth value 0x%x -> 0x%x \n", __PRETTY_FUNCTION__, _vmeth,
         *(unsigned int*)_vmeth, _meth, *_meth);

    /*
     * vtable_ hijacking
     * change its value, now its point to the patch-code method
     */
    *_meth = (void*) _pc;
    //*_meth = (void*) 0xdeadbeef;

    //TODO: change also the declaring_class
    //TODO: hijacking of virtual_methods_ array
    mid = env->GetMethodID(c, t.mname.c_str(), t.msig.c_str() );
    ALOG("%s TRYHARDER found mid : 0x%x \n ", __PRETTY_FUNCTION__, mid);
    originalMethod = (art::mirror::ArtMethod*) mid;
    dec_class = (art::mirror::Class*) originalMethod->declaring_class_;

    unsigned int* _meth2 = ( (unsigned int) originalMethod->declaring_class_ + mid_index * 4 + 0x170 ) ;
    ALOG("%s AFTER vmeth value : 0x%x -> 0x%x, meth value 0x%x -> 0x%x \n", __PRETTY_FUNCTION__,
         dec_class->virtual_methods_, *((unsigned int*)dec_class->virtual_methods_),
         _meth2, *_meth2);

    mid = env->GetMethodID(c, t.mname.c_str(), t.msig.c_str() );
    ALOG("%s TRYHARDER2 found mid : 0x%x \n ", __PRETTY_FUNCTION__, mid);
    return true;
}

void setFieldFlag_5_1(JNIEnv* env, jobject field) {
	art::mirror::ArtField* artField =
			(art::mirror::ArtField*) env->FromReflectedField(field);
	artField->access_flags_ = artField->access_flags_ & (~0x0002) | 0x0001;
}
