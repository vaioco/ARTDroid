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

using namespace std;

namespace artdroid {
    bool isZygote();
    char* _runCommand(string , string );
    string& charToString(char* buf);
    jint getAPIVersion();
    }

#endif //ARTDROID_C_MISC_H
