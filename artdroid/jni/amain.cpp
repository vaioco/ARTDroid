#include "amain.h"

namespace artdroid {

    //ARTDROID ENTRYPOINT
    int artdroid_main( string config_file, string log_file) {
        //set log function used by logger
        set_logfunction(mylog2);
        //create the logger
        Logger* _logger = new Logger(log_file);
        ALOG("%s chiamato!!", __PRETTY_FUNCTION__);
        //resolve symbols from libart.so
        resolveSymbols(&_art_wrapper);
        //get JNIEnv*
        JNIEnv* env = getJNIEnv();
        //read configuration file
        Config* _config = new Config(log_file);
        //using a parser
        Parser* p = new Parser(config_file);
        //data container
        DataStruct* container = new DataStruct();
        container->setContainerInstance(container);
        //populate data containter
        if(! p->parser(container)){
            ALOG("errore");
            return -1;
        }
        container->print();
        //set starting native hooks
        setNativeHooks(*_config);
        //MyDexLoader::set_dexloader(env, *Config::dexfile);

    }
    /*
     * logcat logging function
     */
    void mylog2(char *msg)
    {
        ALOG("%s", msg);
    }
}