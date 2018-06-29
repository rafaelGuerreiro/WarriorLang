//
//  Int8LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef Int8LiteralAST_hpp
#define Int8LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents Int8 literal in WarriorLang:

     ```warriorlang
     let short: Int8 = 127; // Int8 literal
     print(-128 as Int8); // Int8 literal
     ```
     */
    class Int8LiteralAST: public ExpressionAST {
    private:
        int_fast8_t value;
    public:
        ~Int8LiteralAST();
        Int8LiteralAST(int_fast8_t value): value(value) {}
    };
}
#endif /* Int8LiteralAST_hpp */
