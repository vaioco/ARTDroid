LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= artdroid

LOCAL_SRC_FILES:=   amain.cpp \
                    conf/json.c \
                    conf/parser.c \
                    utils/Logger.cpp \
                    conf/AParser.cpp \
                    utils/misc.cpp \
                    structs/art/art_hookhelper.cpp \
                    structs/art/art_hookhelper_5_1.cpp \
                    structs/ArtHook.cpp \
                    structs/Data.cpp \
                    utils/JNIHelper.cpp \
                    utils/ARTStuff.c \
                    utils/MyDexLoader.cpp \
                    nativeHook.cpp \
                    nativeHookARM.cpp.arm \
                    Hooking.cpp

LOCAL_CFLAGS	:= -DDEBUG -O0 -fpermissive
LOCAL_CPPFLAGS  := -std=c++11
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../adbi/instruments/base/
LOCAL_CPP_EXTENSION := .cxx .cpp .cc

#LOCAL_ARM_MODE := thumb

LOCAL_LDLIBS := $(LOCAL_PATH)/../../adbi/instruments/base/obj/local/armeabi/libbase.a
LOCAL_LDLIBS += -llog
#LOCAL_STATIC_LIBRARIES := base
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)