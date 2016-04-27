//
// Created by vaioco on 21/04/16.
//

#ifndef ARTDROID_C_DATA_H
#define ARTDROID_C_DATA_H

#include <map>
#include "structs/ArtHook.h"

using namespace std;


namespace artdroid {

    class ArtHook;

    class DataStruct {
    public:
        map <string, ArtHook> dict;

        void addElement(string key, ArtHook* value);

        ArtHook getElement(string key);

        void print();

        DataStruct() : dict() {};

        void setHooks( void(*foo)(ArtHook& arth) );

        void setContainerInstance(DataStruct*);

        static DataStruct* getContainerInstance();

        const static DataStruct* _inst;
    };

}
#endif //ARTDROID_C_DATA_H
