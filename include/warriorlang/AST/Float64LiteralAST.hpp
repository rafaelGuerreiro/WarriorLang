//
//  NumberExpressionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef Float64LiteralAST_hpp
#define Float64LiteralAST_hpp

#include "ExpressionAST.hpp"

namespace warriorlang {
    /**
     Represents Float64 literal in WarriorLang:

     ```warriorlang
     let double: Float64 = 5.0; // Float64 literal
     print(8.99); // Float64 literal
     ```
     */
    class Float64LiteralAST: public ExpressionAST {
    private:
        double value;
    public:
        ~Float64LiteralAST();
        Float64LiteralAST(double value): value(value) {}
    };
}
#endif /* Float64LiteralAST_hpp */
