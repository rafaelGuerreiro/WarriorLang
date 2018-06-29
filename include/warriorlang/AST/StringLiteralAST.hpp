//
//  StringLiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef StringLiteralAST_hpp
#define StringLiteralAST_hpp

#include "ExpressionAST.hpp"
#include <string>

namespace warriorlang {
    /**
     Represents String literal in WarriorLang:

     ```warriorlang
     let string: String = "text here"; // String literal
     print("The value of the string is: ${string}"); // String literal with interpolation
     print("The value of the string is: $string"); // String literal with interpolation of a simple variable
     ```
     */
    class StringLiteralAST: public ExpressionAST {
    private:
        std::string value;
    public:
        ~StringLiteralAST();
        StringLiteralAST(const std::string &value): value(value) {}
    };
}
#endif /* StringLiteralAST_hpp */
