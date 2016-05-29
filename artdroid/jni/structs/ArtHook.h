//
// Created by vaioco on 20/04/16.
//

#ifndef ARTDROID_C_ARTHOOK_H
#define ARTDROID_C_ARTHOOK_H

#include <string>
#include <iostream>
#include "utils/Logger.h"
#include "utils/Misc.h"

//#include "utils/JNIHelper.h"

using namespace std;

namespace artdroid {
    extern "C" JNIEnv* getJNIEnv();

    /*
     * An hook of a java virtual method
     */
    class ArtHook {
    public:
        /*
         * target class name
         */
        string cname;
        /*
         * target method name
         */
        string mname;
        /*
         * target method signature in Dalvik format
         */
        string msig;
        /*
         * hash = cname + mname + msig
         */
        string hash;
        /*
         * patch-code class name
         */
        string hkname;
        /*
         * pointer (ArtMethod*) to original method
         */
        void* original;
        /*
         * pointer (ArtMethod*) to patch-code method
         */
        void* patch;

        ArtHook(string clsname, string _mname, string _msig, string _hkname, string _hash) : cname(clsname),
                                                               mname(_mname), msig(_msig), hkname(_hkname),
                                                               hash(_hash) { };
        virtual void print();
        /*
         * call the original method using the stored pointer
         */
        virtual void* callOriginalMethod(jobject, jobjectArray);
        virtual jboolean callOriginalMethodZ(jobject, jobjectArray);
    };

}
#endif //ARTDROID_C_ARTHOOK_H
