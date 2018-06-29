//
//  CharacterLiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef CharacterLiteralAST_hpp
#define CharacterLiteralAST_hpp

#include "ExpressionAST.hpp"
#include <string>

namespace warriorlang {
    /**
     Represents Character literal in WarriorLang:

     ```warriorlang
     let string: Character = 'a'; // Character literal
     print('b'); // Character literal
     ```
     */
    class CharacterLiteralAST: public ExpressionAST {
    private:
        char32_t value;
    public:
        ~CharacterLiteralAST();
        CharacterLiteralAST(char32_t value): value(value) {}
    };
}
#endif /* CharacterLiteralAST_hpp */
