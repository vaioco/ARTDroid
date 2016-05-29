//
// Created by vaioco on 19/04/16.
//

#ifndef ARTDROID_C_MISC_H
#define ARTDROID_C_MISC_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include "utils/Logger.h"
#include "utils/JNIHelper.h"
#include "Globals.h"
#include "structs/ArtHook.h"

using namespace std;

namespace artdroid {
    class ArtHook;
    static string BRIDGE_UTILS = "org/sid/arthookbridge/Utils";

    bool isZygote();
    char* _runCommand(string , string );
    string& charToString(char* buf);
    jint getAPIVersion();
    char* charFromJstring(JNIEnv* , jstring s);
    string ConvertJString(JNIEnv*, jstring);

    jboolean callGetBoolean(JNIEnv* env, jobject javaArgs, int index);
    jdouble callGetDouble(JNIEnv* env, jobject javaArgs, int index);
    jfloat callGetFloat(JNIEnv* env, jobject javaArgs, int index);
    jbyteArray callGetByteArray(JNIEnv* env, jobject javaArgs, int index);
    jlong callGetLong(JNIEnv* env, jobject javaArgs, int index);
    jint callGetInt(JNIEnv* env, jobject javaArgs, int index);
    jobject callGetObj(JNIEnv* env, jobject javaArgs, int index);
    jvalue* tryToUnbox(JNIEnv* env, ArtHook* hook, unsigned int* javaArgs,jobject thiz, bool call_patchmeth);
    static void _helper(JNIEnv* env, char *src, char* result, bool array, jvalue* args, int counter,
                        int index, jobjectArray joa);
    char* parseSignature(JNIEnv* env, char* sig, jvalue* args, jobjectArray joa, int counter);
    void parseReturnType(JNIEnv* env, char *, char*);
}


#endif //ARTDROID_C_MISC_H
