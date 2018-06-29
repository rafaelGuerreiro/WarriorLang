//
//  Float32LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef Float32LiteralAST_hpp
#define Float32LiteralAST_hpp

#include "ExpressionAST.hpp"

namespace warriorlang {
    /**
     Represents Float32 literal in WarriorLang:

     ```warriorlang
     let float: Float32 = 5.0; // Float32 literal
     print(6.13 as Float32); // Float32 literal
     ```
     */
    class Float32LiteralAST: public ExpressionAST {
    private:
        float value;
    public:
        ~Float32LiteralAST();
        Float32LiteralAST(float value): value(value) {}
    };
}
#endif /* Float32LiteralAST_hpp */
