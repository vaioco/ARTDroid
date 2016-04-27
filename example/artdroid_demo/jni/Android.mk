LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= libhook
LOCAL_SRC_FILES := usermain.cpp
LOCAL_CFLAGS	:= -DDEBUG -O0 -fpermissive
LOCAL_CPPFLAGS  := -std=c++11
LOCAL_C_INCLUDES := ../../../adbi/instruments/base/ ../../../artdroid/jni/
LOCAL_LDLIBS    := -ldl -llog
LOCAL_LDLIBS += -Wl,--start-group $(LOCAL_PATH)/../../../adbi/instruments/base/obj/local/armeabi/libbase.a $(LOCAL_PATH)/../../../artdroid/obj/local/armeabi-v7a/libartdroid.a -Wl,--end-group
LOCAL_LDLIBS += -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.9/libs/$(TARGET_ARCH_ABI) -lgnustl_static
LOCAL_ARM_MODE := thumb
include $(BUILD_SHARED_LIBRARY)


