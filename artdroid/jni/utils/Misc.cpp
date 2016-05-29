//
// Created by vaioco on 19/04/16.
//

#include "Misc.h"
#include "sys/stat.h"
#include "errno.h"

namespace artdroid /**/{

    jint getAPIVersion()
    {
        JNIEnv* my_env = nullptr;
        ALOG("%s CALLED!!!\n", __PRETTY_FUNCTION__);
        my_env = getJNIEnv();
        if( my_env == nullptr){
            ALOG("ERROR %s getting jni\n", __PRETTY_FUNCTION__);
            return 0;
        }
        jclass versionClass = my_env->FindClass("android/os/Build$VERSION");
        jfieldID sdkIntFieldID = my_env->GetStaticFieldID(versionClass, "SDK_INT", "I");
        jint sdkInt = my_env->GetStaticIntField(versionClass, sdkIntFieldID);
        ALOG("API version: %d \n", sdkInt);
        return sdkInt;
    }
    string ConvertJString(JNIEnv* env, jstring s)
    {
        if(!s) return string();
        const char* _s = charFromJstring(env,s);
        ALOG("%s converting : %s \n", __PRETTY_FUNCTION__, _s);
        string res = charToString(_s);
        ALOG("%s res: %s \n", __PRETTY_FUNCTION__, res.c_str() );
        env->ReleaseStringUTFChars(s, _s);
        return res;
    }
    char* charFromJstring(JNIEnv* env, jstring s)
    {
        ALOG("jsing value: %x \n", s);
        jsize len = env->GetStringUTFLength(s);
        char* _string = calloc(len + 1, 1);
        env->GetStringUTFRegion(s, 0, len, _string);
        return _string;
    }
    bool isZygote()
    {
        bool res = 0;
        string cmd_isZygote = "ps | grep zygote | tr -s \" \" | cut -d \" \" -f 2";
        string e;
        ALOG("%s running command: %s \n", __PRETTY_FUNCTION__, cmd_isZygote.c_str());

        char *c = _runCommand(cmd_isZygote, e);
        ALOG("command returned: %s \n", c);
        int mpidZygote = atoi(c);
        if ( ::getpid() == mpidZygote) {
            res = 1;
        }
        return res;
    }
    string& charToString(char* buf)
    {
        stringstream tmp;
        string dex;
        tmp << buf;
        tmp >> dex;
        return dex;
    }
    char* _runCommand(string command, string searchme)
    {
        char *buffer  = (char*) calloc(1,128);
        FILE* fp = popen(command.c_str(), "r");
        if(fp == NULL){
            ALOG("ERROR getprop\n");
            return nullptr;
        }
        while( !feof(fp) ){
            if( fgets(buffer, 128, fp) != NULL){/**/
                //arthooklog("%s buffer = %s \n", __PRETTY_FUNCTION__, buffer);
                if(!searchme.empty()) {
                    if (strstr(buffer, searchme.c_str()) != NULL) return (char *) 1;
                }else {
                    ALOG("%s AAAA ritorno %s\n",__PRETTY_FUNCTION__, buffer);
                    return buffer;
                }
            }else{
                ALOG("%s cannot read from fp!\n", __PRETTY_FUNCTION__);
                pclose(fp);
                return nullptr;
            }
        }
        pclose(fp);
        ALOG("%s AAAA2 ritorno %s\n",__PRETTY_FUNCTION__, buffer);
        return buffer;
    }
    jvalue* tryToUnbox(JNIEnv* env, ArtHook* hook, unsigned int* javaArgs,jobject thiz, bool call_patchmeth)
    {
        if(!javaArgs) return nullptr;
        ALOG("%s called with : %p \n", __PRETTY_FUNCTION__, javaArgs);
        jobjectArray joa = (jobjectArray) javaArgs;
        int num = env->GetArrayLength( joa);
        ALOG("%s array con numero elementi: %d, bool  = %d  !!!! \n", __PRETTY_FUNCTION__, num, call_patchmeth);
        jvalue *args = NULL;
        int counter = 0;
        if (num == 0 && !call_patchmeth)
            return args;
        if (call_patchmeth)
            num += 1;
        args = calloc(num, sizeof(jvalue));
        if (call_patchmeth) {
            args[0].l = thiz;
            counter++;
        }
        ALOG("%s  signature: %s\n", __PRETTY_FUNCTION__, hook->msig.c_str());
        parseSignature(env, hook->msig.c_str(), args, joa, counter);
        return args;
    }
    static void _helper(JNIEnv* env, char *src, char* result, bool array, jvalue* args,
                        int counter, int index, jobjectArray joa)
    {
        if (!src) return;
        ALOG("src vale: %c, result: %s \n", *src, result);
        char* res = nullptr;
        switch (*src) {
            case ')':
                break;
            case 'B':
                if (array) {
                    strcat(result, "[B|");
                    ALOG("parser sto chiamanto getbytearray su %c con index = %d\n", *src, index);
                    args[counter].l = callGetByteArray(env, joa, (jint) index);
                    counter++;index++;
                }
                else {
                    strcat(result, "B|");
                }
                *src++;
                return _helper(env, src, result, 0, args, counter, index,joa);
                break;
            case 'D':
                if (array) {
                    strcat(result, "[D|");
                }
                else {
                    strcat(result, "D|");
                    args[counter].d = callGetDouble(env, joa, (jint) index);
                    counter++;index++;
                }
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
                break;
            case 'Z':
                if (array)
                    strcat(result, "[Z|");
                else {
                    strcat(result, "Z|");
                    args[counter].z = callGetBoolean(env, joa, (jint) index);
                    counter++;index++;
                }
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
                break;
            case 'J':
                if (array)
                    strcat(result, "[J|");
                else
                    strcat(result, "J|");
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
                break;
            case 'I':
                if (array)
                    strcat(result, "[I|");
                else {
                    strcat(result, "I|");
                    ALOG("parser sto chiamanto getint su %c con index = %d \n", *src, index);
                    args[counter].i = callGetInt(env, joa, (jint) index);
                    counter++;index++;
                }
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
                break;
            case '[':
                *src++;
                return _helper(env,src, result, 1, args, counter, index,joa);
                break;
            case 'L':
                if (array) {
                    strcat(result, "[L|");
                    args[counter].l = callGetObj(env, joa, (jint) index) ;
                    index++; counter++;
                }
                else {
                    strcat(result, "L|");
                    ALOG("%s trovato arg L, chiamo java helper\n", __PRETTY_FUNCTION__);
                    args[counter].l = callGetObj(env, joa, (jint) index);
                    counter++;index++;
                }
                //search end obj
                res = strchr(src, ';');
                //goto end
                src = res;
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
            default:
                *src++;
                return _helper(env,src, result, 0, args, counter, index,joa);
        }
    }
    char* parseSignature(JNIEnv* env, char* sig, jvalue* args, jobjectArray joa, int counter)
    {
        char* p = sig;
        char* result = calloc(strlen(sig)+1, sizeof(char));
        result[strlen(sig)+1] = 0x0;
        _helper(env, p,result,0,args, counter, 0, joa);
        ALOG("%s result: %s \n", __PRETTY_FUNCTION__, result);
        free(result);
    }
    void parseReturnType(JNIEnv* env, char *sig, char* retType)
    {
        ALOG("%s sig: %s \n", __PRETTY_FUNCTION__, sig);
        char* p = strchr(sig, ')');
        ALOG("%s sig: %s \n", __PRETTY_FUNCTION__, p);
        p++;
        ALOG("%s switch on %c \n", __PRETTY_FUNCTION__, *p);
        switch(*p){
            case 'V':
                //callOriginalVoidMethod(env, thiz, c, mid, args);
                strncpy(retType, "V",1);
                ALOG("%s copiato V dentro: %c \n", __PRETTY_FUNCTION__, *retType);
                break;
            case 'L':
                strncpy(retType, "L",1);
                ALOG("%s copiato L dentro: %c \n", __PRETTY_FUNCTION__, *retType);
                break;
            case 'Z':
                strncpy(retType, "Z", 1);
                break;
            default:
                break;
        }
    }
    jboolean callGetBoolean(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getBoolean", "([Ljava/lang/Object;I)Z");
        jboolean res = env->CallStaticIntMethod(test, mid, javaArgs, index);
        return res;

    }
    jdouble callGetDouble(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getDouble", "([Ljava/lang/Object;I)D");
        jdouble res = env->CallStaticDoubleMethod(test, mid, javaArgs, index);
        return res;

    }
    jfloat callGetFloat(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getFloat", "([Ljava/lang/Object;I)F");
        jfloat res = env->CallStaticFloatMethod(test, mid, javaArgs, index);
        return res;
    }
    jbyteArray callGetByteArray(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getByteArray", "([Ljava/lang/Object;I)[B");
        jbyteArray res = env->CallStaticObjectMethod(test, mid, javaArgs, index);
        return res;
    }
    jlong callGetLong(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getLong", "([Ljava/lang/Object;I)J");
        jlong res = env->CallStaticFloatMethod(test, mid, javaArgs, index);
        return res;

    }
    jint callGetInt(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = findClassFromClassLoader(env,gDexLoader,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getInt", "([Ljava/lang/Object;I)I");
        jint res = env->CallStaticIntMethod(test, mid, javaArgs, index);
        return res;

    }
    jobject callGetObj(JNIEnv* env, jobject javaArgs, int index)
    {
        jclass test = _findClassFromDexCL(env,BRIDGE_UTILS);
        jmethodID mid = env->GetStaticMethodID(test, "getObj", "([Ljava/lang/Object;I)Ljava/lang/Object;");
        ALOG("%s chiamo getobject helper\n", __PRETTY_FUNCTION__);
        jobject res = env->CallStaticObjectMethod(test, mid, javaArgs, index);
        return env->NewGlobalRef(res);

    }
    char* _config_create_env()
    {
        char tmpdir[256];
        char ppid[8];

        ALOG("dentro %s, starting %d ....\n", __PRETTY_FUNCTION__, getpid());

        sprintf(tmpdir,"%s/","/data/local/tmp/dex/opt");
        //zygote fix
        //entro dentro la cartella tmp_root e creo una mia dir
        if( chdir("/data/local/tmp/dex/opt") ){
            ALOG("ERROR chdir!!\n");
            return 0;
        }
        sprintf(ppid,"%d",getpid());
        strcat(tmpdir,ppid);
        ALOG("creo working dir: %s \n", tmpdir);
        if( mkdir(tmpdir, 0777) != 0 ){
            ALOG("ERROR mkdir: %s \n", strerror(errno));
            return 0;
        }
        if( chdir(tmpdir) ){
            ALOG("ERROR chdir!!\n");
            return 0;
        }
        return tmpdir;
    }

}