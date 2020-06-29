//
// Created by parallels on 6/29/20.
//

#include "util.h"
namespace mylibco{
    void Backtrace(std::vector<std::string> &bt ,int size,int skip){
        void ** arraya = static_cast<void **>(malloc(sizeof(void *) * size));
        size_t len = backtrace(arraya,size);
        char** strings = backtrace_symbols(arraya,len);
        if(strings == nullptr){

        }
        for(size_t i= skip; i < len;++i) {
            if(strings)
            bt.push_back(strings[i]);
        }

        free(strings);
        free(arraya);
    }

    std::string BackTraceToString(int size,int skip,const std::string& prefix){
        std::vector<std::string> bt;
        Backtrace(bt,size,skip);
        std:: stringstream ss;
        for(const auto & str:bt){
            ss << prefix << str << "\n";
        }
        return ss.str();
    }
}
