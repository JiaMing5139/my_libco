//
// Created by parallels on 6/29/20.
//

#ifndef MY_LIBCO_UTIL_H
#define MY_LIBCO_UTIL_H

#include <vector>
#include <string>
#include <execinfo.h>
#include <sstream>
namespace mylibco{
    void Backtrace(std::vector<std::string> &bt ,int size,int skip);
    std::string BackTraceToString(int size,int skip,const std::string& prefix);
}

#endif //MY_LIBCO_UTIL_H
