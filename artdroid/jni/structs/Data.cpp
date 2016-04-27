//
// Created by vaioco on 21/04/16.
//

#include "Data.h"


namespace artdroid{

    const DataStruct* DataStruct::_inst = nullptr;

    void DataStruct::addElement(string key, ArtHook* value){
        //dict[key] = *value;
        dict.insert(std::map<string, ArtHook>::value_type (key,*value));
    }
    void DataStruct::print(){
        typedef std::map<std::string, ArtHook>::iterator it_type;
        for(it_type iterator = this->dict.begin(); iterator != this->dict.end(); iterator++) {
            // iterator->first = key
            // iterator->second = value
            // Repeat if you also want to iterate through the second map.
            ALOG("%s key: %s", __PRETTY_FUNCTION__, iterator->first.c_str());
        }
    }

    void DataStruct::setHooks( void(*foo)(ArtHook& arth) ){

        ALOG("%s called with %x\n", __PRETTY_FUNCTION__, this);
        typedef std::map<std::string, ArtHook>::iterator it_type;
        for(it_type iterator = this->dict.begin(); iterator != this->dict.end(); iterator++) {
            // iterator->first = key
            // iterator->second = value
            // Repeat if you also want to iterate through the second map.
            ALOG("%s key: %s", __PRETTY_FUNCTION__, iterator->first.c_str());
            foo(iterator->second);
        }
    }

    void DataStruct::setContainerInstance(DataStruct* c){
        DataStruct::_inst = c;
    }
    static DataStruct* DataStruct::getContainerInstance(){
        return DataStruct::_inst;
    }
}