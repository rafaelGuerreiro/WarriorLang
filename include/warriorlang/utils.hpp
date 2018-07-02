//
//  utils.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-07-01.
//

#ifndef utils_hpp
#define utils_hpp

#include <string>

namespace warriorlang {
    void exceptionHandler(int sig);
    void safelyDeletePointer(void *pointer);
    bool stringContains(const std::string &str, const char &c);
}

#endif /* utils_cpp */