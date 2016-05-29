//
// Created by vaioco on 21/03/16.
//

#include "main.h"

using namespace std;
using namespace artdroid;


__attribute__ ((constructor)) void my_init(void) {
    ALOG("lib constructor");
    string conf = "/data/local/tmp/test.json";
    string logfile = "/data/local/tmp/artdroid.log";
    artdroid_main(conf, logfile);
}