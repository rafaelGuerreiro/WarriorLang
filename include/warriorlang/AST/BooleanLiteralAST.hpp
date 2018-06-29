//
//  BooleanLiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef BooleanLiteralAST_hpp
#define BooleanLiteralAST_hpp

#include "ExpressionAST.hpp"

namespace warriorlang {
    /**
     Represents Boolean literal in WarriorLang:

     ```warriorlang
     let boolean: Boolean = false; // Boolean literal
     print(true); // Boolean literal
     ```
     */
    class BooleanLiteralAST: public ExpressionAST {
    private:
        bool value;
    public:
        ~BooleanLiteralAST();
        BooleanLiteralAST(bool value): value(value) {}
    };
}
#endif /* BooleanLiteralAST_hpp */
