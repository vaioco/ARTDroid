//
// Created by vaioco on 21/04/16.
//

#include "Data.h"


namespace artdroid{

    const DataStruct* DataStruct::_inst = nullptr;

    void DataStruct::addElement(string key, ArtHook* value){
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

    ArtHook* DataStruct::search(const string _k){
        //ALOG("%s searching key: %s \n", __PRETTY_FUNCTION__, _k.c_str() );
        typedef std::map<std::string, ArtHook>::iterator it_type;
        for(it_type iterator = this->dict.begin(); iterator != this->dict.end(); iterator++) {
            // iterator->first = key
            // iterator->second = value
            // Repeat if you also want to iterate through the second map.
            if(_k == iterator->first)
            {
                //ALOG("%s found KEY %s !!!\n", __PRETTY_FUNCTION__, iterator->first.c_str() );
                return &iterator->second;
            }
        }
    }
    /*
     * iterate over the map, calling the callback on each entry
     */
    void DataStruct::setHooks( void(*foo)(ArtHook& arth) ){
        ALOG("%s called with %x\n", __PRETTY_FUNCTION__, this);
        typedef std::map<std::string, ArtHook>::iterator it_type;
        for(it_type iterator = this->dict.begin(); iterator != this->dict.end(); iterator++) {
            // iterator->first = key
            // iterator->second = value
            // Repeat if you also want to iterate through the second map.
            //ALOG("%s key: %s", __PRETTY_FUNCTION__, iterator->first.c_str());
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