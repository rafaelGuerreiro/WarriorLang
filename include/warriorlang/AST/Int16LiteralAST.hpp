//
//  Int16LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef Int16LiteralAST_hpp
#define Int16LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents Int16 literal in WarriorLang:

     ```warriorlang
     let short: Int16 = 10_000; // Int16 literal
     print(-1_000 as Int16); // Int16 literal
     ```
     */
    class Int16LiteralAST: public ExpressionAST {
    private:
        int_fast16_t value;
    public:
        ~Int16LiteralAST();
        Int16LiteralAST(int_fast16_t value): value(value) {}
    };
}
#endif /* Int16LiteralAST_hpp */
