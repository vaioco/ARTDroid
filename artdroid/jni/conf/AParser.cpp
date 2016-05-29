//
// Created by vaioco on 19/04/16.
//

#include "AParser.h"


namespace artdroid{

    static bool _DEBUG = false;

    const Config* Config::_inst = nullptr;

    void Config::setConfigInstance(Config* c){
        Config::_inst = c;
    }
    static Config* Config::getConfigInstance(){
        return Config::_inst;
    }

    void Parser::createHook(json_value* jobj, int len, DataStruct* d){
        char *cname = jobj->u.object.values[0].value->u.string.ptr;
        char *mname = jobj->u.object.values[1].value->u.string.ptr;
        char *msig = jobj->u.object.values[2].value->u.string.ptr;
        char *hookclsname = jobj->u.object.values[3].value->u.string.ptr;
        string _cname = charToString(cname);
        string _mname = charToString(mname);
        string _msig = charToString(msig);
        string _hash;
        string _hkname = charToString(hookclsname);
        std::stringstream ss;
        ss << _cname << _mname << _msig;
        _hash = ss.str();
        ArtHook* hook = new ArtHook(_cname, _mname, _msig, _hkname, _hash);
        hook->print();
        d->addElement(_hash, hook);
    }
    bool Config::readConfig(){
        ALOG("%s \n", __PRETTY_FUNCTION__);
        bool zyg = isZygote();
        jint apiv = getAPIVersion();
        this->zygote = zyg;
        this->apiversion = (int) apiv;
        return true;
    }

    bool Parser::parsing_hook_objects(json_value* value, int depth, DataStruct* d){
        int length,x,i,l;
        json_value* root = NULL;
        json_value* tmp = NULL;

        length = value->u.array.length;
        for(x = 0; x < length; x++){
            tmp = value->u.array.values[x];
            l = tmp->u.object.length;
            for ( i = 0; i < l ;i++){
                ALOG("name: %s  = %s \n", tmp->u.object.values[i].name,tmp->u.object.values[i].value->u.string.ptr );
            }
            createHook(tmp, l,d);
        }
        return 0;
    }
    void Config::print(){
        ALOG("printing config: \n");
        ALOG("zygote: %d \n", this->zygote);
        ALOG("apiversion: %d \n", this->apiversion);
    }
    Config::Config(string fname){
        this->log_fname = fname;
        this->readConfig();
        this->print();
        Config::setConfigInstance(this);
    }

    string* Config::dexfile = new string();
    bool Config::_DEBUG = false;

    static void Config::setDexFile(string* dex){
        ALOG("%s value: %s\n", __PRETTY_FUNCTION__, dex->c_str());
        Config::dexfile = dex;
        ALOG("%s end value: %s\n", __PRETTY_FUNCTION__, dex->c_str());
    }
    static void Config::setDebug(bool value){
        Config::_DEBUG = value;
    }
    bool Parser::start_parsing(json_value* value, DataStruct* d){
        json_value* root = NULL;

        if (value == NULL)
            return false;
        if( strcmp(value->u.object.values[0].name, "config") != 0){
            return false;
        }
        root = value->u.object.values[0].value; //debug object
        int debug = root->u.object.values[0].value->u.integer;
        Config::setDebug((bool)debug);
        char* dexfile = root->u.object.values[1].value->u.string.ptr;
        std::stringstream tmp;
        string* dex = new string();
        tmp << dexfile;
        tmp >> *dex;
        ALOG("dexfile: %s\n", dex->c_str());
        Config::setDexFile(dex);
        root = root->u.object.values[2].value;
        parsing_hook_objects(root, 2, d);
    }

    bool Parser::parser(DataStruct* d){
        ALOG("parser started!");
        struct stat file_status;
        int64_t file_size = 0;
        FILE* fp = NULL;
        json_char* json;
        json_value* node;

        if( stat(this->fname.c_str(), &file_status) != 0  ) {
            ALOG("parser error!!");
            return false;
        }
        file_size = file_status.st_size;
        char* file_contents = (char*) malloc(file_size);
        if( file_contents == NULL){
            ALOG("ERROR malloc \n");
            return false;
        }
        fp = fopen(this->fname.c_str(), "rt");
        if(!fp){
            ALOG("ERROR fopen %s \n", this->fname.c_str());
            free(file_contents);
            return false;
        }
        if ( fread(file_contents, file_size,1,fp) != 1){
            ALOG("ERROR fread\n");
            free(file_contents);
            fclose(fp);
            return false;
        }
        fclose(fp);
        json = (json_char*) file_contents;
        node = json_parse(json,file_size);
        if(node == NULL){
            ALOG("ERROR while parsing %s \n", this->fname.c_str());
            free(file_contents);
            fclose(fp);
            return false;
        }
        start_parsing(node, d);
        json_value_free(node);
        free(file_contents);
        return true;
    }

}