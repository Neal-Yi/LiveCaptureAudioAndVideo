#include "Layer.h"
#include <fstream>

void printInfo(Info *t,std::ofstream *f){
    if(t == NULL)return;
    while(!t->name.empty()){

        (*f)<<t->name.front()<<":";
        t->name.pop();
    }
    while(!t->srcInfo.empty()){
        (*f)<<t->srcInfo.front();
        t->srcInfo.pop();
    }
    (*f)<<"->";
    while(!t->dstInfo.empty()){
        (*f)<<t->dstInfo.front();
        t->dstInfo.pop();
    }
    (*f)<<std::endl;
}
