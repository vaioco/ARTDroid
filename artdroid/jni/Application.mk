APP_ABI := armeabi-v7a
APP_PLATFORM := android-19
APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION := 4.9
APP_MODULE := libartdroid libhook

#  Enable C++11. However, pthread, rtti and exceptions aren’t enabled
#APP_CFLAGS += -Wno-error=format-security
