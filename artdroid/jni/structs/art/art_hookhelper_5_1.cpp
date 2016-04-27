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

void hook_5_1(JNIEnv* env, jobject src, jobject dest) {
	art::mirror::ArtMethod* smeth =
			(art::mirror::ArtMethod*) env->FromReflectedMethod(src);

	art::mirror::ArtMethod* dmeth =
			(art::mirror::ArtMethod*) env->FromReflectedMethod(dest);

	dmeth->declaring_class_->class_loader_ =
			smeth->declaring_class_->class_loader_; //for plugin classloader
	dmeth->declaring_class_->clinit_thread_id_ =
			smeth->declaring_class_->clinit_thread_id_;
	dmeth->declaring_class_->status_ = smeth->declaring_class_->status_-1;

	smeth->declaring_class_ = dmeth->declaring_class_;
	smeth->dex_cache_resolved_types_ = dmeth->dex_cache_resolved_types_;
	smeth->access_flags_ = dmeth->access_flags_;
	smeth->dex_cache_resolved_methods_ = dmeth->dex_cache_resolved_methods_;
	smeth->dex_code_item_offset_ = dmeth->dex_code_item_offset_;
	smeth->method_index_ = dmeth->method_index_;
	smeth->dex_method_index_ = dmeth->dex_method_index_;

	smeth->ptr_sized_fields_.entry_point_from_interpreter_ =
			dmeth->ptr_sized_fields_.entry_point_from_interpreter_;

	smeth->ptr_sized_fields_.entry_point_from_jni_ =
			dmeth->ptr_sized_fields_.entry_point_from_jni_;
	smeth->ptr_sized_fields_.entry_point_from_quick_compiled_code_ =
			dmeth->ptr_sized_fields_.entry_point_from_quick_compiled_code_;

	ALOG("replace_5_1: %d , %d",
			smeth->ptr_sized_fields_.entry_point_from_quick_compiled_code_,
			dmeth->ptr_sized_fields_.entry_point_from_quick_compiled_code_);

}

void test_5_1(JNIEnv* env, artdroid::ArtHook& t){
    ALOG("%s searching class: %s\n", __PRETTY_FUNCTION__, t.cname.c_str());
    jclass c = env->FindClass(t.cname.c_str());
    ALOG("%s found class : %x \n ", __PRETTY_FUNCTION__, c);
    void* mid = env->GetMethodID(c, t.mname.c_str(), t.msig.c_str() );
    ALOG("%s found mid : 0x%x \n ", __PRETTY_FUNCTION__, mid);


    ALOG("%s searching patchcode class: %s\n", __PRETTY_FUNCTION__, t.hkname.c_str());
    jclass c2 = artdroid::_findClassFromDexCL(env, t.hkname);
    ALOG("%s found patchcode class : %x \n ", __PRETTY_FUNCTION__, c2);

    unsigned int* _pc = (unsigned int *) (env)->GetMethodID(c2, t.mname.c_str(), t.msig.c_str());

    art::mirror::ArtMethod* patchMethod = (art::mirror::ArtMethod*) _pc;
	art::mirror::ArtMethod* originalMethod = (art::mirror::ArtMethod*) mid;

    uint32_t mid_index = originalMethod->method_index_;
    uint32_t mid_index2 = patchMethod->method_index_;

    ALOG("%s found patch mid : 0x%x -> 0x%x , index: %d \n ", __PRETTY_FUNCTION__, _pc,  *_pc, mid_index2);

    ALOG("%s class value : 0x%x \n", __PRETTY_FUNCTION__, *originalMethod->declaring_class_);
    ALOG("%s mid_index value : 0x%x \n", __PRETTY_FUNCTION__, mid_index);

    ALOG("%s swapping declaring class: ");
    ALOG("0x%x <- 0x%x \n", patchMethod->declaring_class_ , originalMethod->declaring_class_);
    //*(unsigned int*)patchMethod->declaring_class_ = (unsigned int*) originalMethod->declaring_class_;

    art::mirror::Class* dec_class = (art::mirror::Class*) originalMethod->declaring_class_;
    unsigned int* _vmeth = dec_class->virtual_methods_;
    unsigned int* _meth = ( (unsigned int) originalMethod->declaring_class_ + mid_index * 4 + 0x170 ) ;
    ALOG("%s BEFORE vmeth value : 0x%x -> 0x%x , meth value 0x%x -> 0x%x \n", __PRETTY_FUNCTION__, _vmeth,
         *(unsigned int*)_vmeth, _meth, *_meth);

    *_meth = (void*) _pc;

    mid = env->GetMethodID(c, t.mname.c_str(), t.msig.c_str() );
    ALOG("%s TRYHARDER found mid : 0x%x \n ", __PRETTY_FUNCTION__, mid);
    originalMethod = (art::mirror::ArtMethod*) mid;
    dec_class = (art::mirror::Class*) originalMethod->declaring_class_;

    unsigned int* _meth2 = ( (unsigned int) originalMethod->declaring_class_ + mid_index * 4 + 0x170 ) ;
    ALOG("%s AFTER vmeth value : 0x%x -> 0x%x, meth value 0x%x -> 0x%x \n", __PRETTY_FUNCTION__,
         dec_class->virtual_methods_, *((unsigned int*)dec_class->virtual_methods_),
         _meth2, *_meth2);

    //*_meth = 0xdeadbeef;
}

void setFieldFlag_5_1(JNIEnv* env, jobject field) {
	art::mirror::ArtField* artField =
			(art::mirror::ArtField*) env->FromReflectedField(field);
	artField->access_flags_ = artField->access_flags_ & (~0x0002) | 0x0001;
}
