#include "artd_main.h"

namespace artdroid {

    int artdroid_main( string config_file, string log_file) {
        set_logfunction(mylog2);
        Logger* _logger = new Logger(log_file);
        ALOG("%s chiamato!!", __PRETTY_FUNCTION__);
        resolveSymbols(&_art_wrapper);
        JNIEnv* env = getJNIEnv();
        Config* _config = new Config(log_file);
        _config->setConfigInstance(_config);
        Parser* p = new Parser(config_file);
        DataStruct* container = new DataStruct();
        container->setContainerInstance(container);

        if(! p->parser(container)){
            ALOG("errore");
            return -1;
        }
        container->print();
        setNativeHooks(*_config);
        //MyDexLoader::set_dexloader(env, *Config::dexfile);
    }
    void mylog2(char *msg)
    {
        ALOG("%s", msg);
    }
    /*
    __attribute__ ((constructor)) void my_init(void) {
        ALOG("lib constructor");
        string conf = "/data/local/tmp/test.json";
        string logfile = "/data/local/tmp/artdroid.log";
        artdroidmain(conf, logfile);
    }
     */
}