//
// Created by vaioco on 22/04/16.
//

#include "MyDexLoader.h"


namespace artdroid{

    static jobject MyDexLoader::set_dexloader(JNIEnv* env, string& dexfile)
    {
        ALOG("%s CALLED \n", __PRETTY_FUNCTION__);
        JNIEnv* myenv = getJNIEnv();
        jobject systemCL = _getSystemClassLoader();
        string a = "";
        jobject dexloader  = _createDexClassLoader(systemCL, dexfile, a);
        jobject gDexLoader = myenv->NewGlobalRef( dexloader);
        jclass c2 = _loadClassFromClassLoader(dexloader, a );
        return gDexLoader;
    }

}