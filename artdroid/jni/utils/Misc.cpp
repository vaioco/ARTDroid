//
// Created by vaioco on 19/04/16.
//

#include "Misc.h"

namespace artdroid {

    jint getAPIVersion(){
        JNIEnv* env;
        ALOG("%s CALLED!!!\n", __PRETTY_FUNCTION__);
        if( _env == nullptr){
            env = getJNIEnv();
            if( env == nullptr){
                ALOG("ERROR %s getting jni\n", __PRETTY_FUNCTION__);
                return 0;
            }
        }
        jclass versionClass = env->FindClass("android/os/Build$VERSION");
        jfieldID sdkIntFieldID = env->GetStaticFieldID(versionClass, "SDK_INT", "I");
        jint sdkInt = env->GetStaticIntField(versionClass, sdkIntFieldID);
        ALOG("API version: %d \n", sdkInt);
        return sdkInt;
    }
    bool isZygote() {
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

    string& charToString(char* buf){
        stringstream tmp;
        string dex;
        tmp << buf;
        tmp >> dex;
        return dex;
    }
    char* _runCommand(string command, string searchme){
        char *buffer  = (char*) calloc(1,128);
        FILE* fp = popen(command.c_str(), "r");
        if(fp == NULL){
            ALOG("ERROR getprop\n");
            return nullptr;
        }
        while( !feof(fp) ){
            if( fgets(buffer, 128, fp) != NULL){
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
}