//
// Created by vaioco on 20/04/16.
//

#ifndef ARTDROID_C_ARTHOOK_H
#define ARTDROID_C_ARTHOOK_H

#include <string>
#include <iostream>
#include "utils/Logger.h"

using namespace std;

namespace artdroid {
    class ArtHook {
    public:
        string cname;
        string mname;
        string msig;
        string hash;
        string hkname;
        void* original;

        ArtHook(string clsname, string _mname, string _msig, string _hkname, string _hash) : cname(clsname),
                                                               mname(_mname), msig(_msig), hkname(_hkname),
                                                               hash(_hash) { };
        virtual void print();
        bool startHooking();
    };

}
#endif //ARTDROID_C_ARTHOOK_H
