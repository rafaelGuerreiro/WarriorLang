//
//  WarLangToken.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef WarLangToken_hpp
#define WarLangToken_hpp

#include "TokenCategory.hpp"
#include <string>

namespace warriorlang {
    class WarLangToken {
    private:
        std::string file;
        unsigned int line;
        TokenCategory category;
        std::string value;
    public:
        ~WarLangToken();
        WarLangToken(const std::string &file,
              unsigned int line,
              TokenCategory category,
              const std::string &value) {
            this->file = file;
            this->line = line;
            this->category = category;
            this->value = value;
        }
    };
}

#endif /* WarLangToken_hpp */
