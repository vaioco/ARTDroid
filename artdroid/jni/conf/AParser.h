//
// Created by vaioco on 19/04/16.
//

#ifndef ARTDROID_C_APARSER_H
#define ARTDROID_C_APARSER_H

#include <sys/stat.h>
#include <string>
#include <sstream>

#include "utils/Logger.h"
#include "conf/json.h"
#include "utils/Misc.h"
#include "structs/ArtHook.h"
#include "structs/Data.h"

using namespace std;



namespace artdroid {
    class DataStruct;

    class Config {

    public:
        static string* dexfile;
        static bool _DEBUG;
        int64_t version;
        string log_fname;
        string optdir;
        int apiversion;
        bool zygote = false;
        Config(string fname);
        bool readConfig();
        void print();
        static void setDebug(bool value);
        static void setDexFile(string* dex);
        void setConfigInstance(Config* c);
        static Config* getConfigInstance();
        const static Config* _inst;
    };

    class Parser {
    public:
        string fname;
        bool parser(DataStruct* c);
        bool start_parsing(json_value* value, DataStruct* d);
        bool parsing_hook_objects(json_value* value, int depth, DataStruct* d);
        Parser(string fn) : fname(fn){};
        void createHook(json_value* jobj, int len, DataStruct* d);
    };
}


#endif //ARTDROID_C_APARSER_H
