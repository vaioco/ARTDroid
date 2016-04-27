//
// Created by vaioco on 20/04/16.
//

#include "ArtHook.h"
namespace artdroid{
    void ArtHook::print(){
        string text ;
        text += "Hook: clsname: ";
        text += this->cname;
        text += " mname: ";
        text += this->mname;
        text += " hash: ";
        text += this->hash;
        ALOG("%s : %s\n", __PRETTY_FUNCTION__, text.c_str());
    }

    void startHooking(){

    }
}