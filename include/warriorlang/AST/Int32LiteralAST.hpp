//
//  Int32LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef Int32LiteralAST_hpp
#define Int32LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents Int32 literal in WarriorLang:

     ```warriorlang
     let short: Int32 = 10_000; // Int32 literal
     print(-1_000 as Int32); // Int32 literal
     ```
     */
    class Int32LiteralAST: public ExpressionAST {
    private:
        int_fast32_t value;
    public:
        ~Int32LiteralAST();
        Int32LiteralAST(int_fast32_t value): value(value) {}
    };
}
#endif /* Int32LiteralAST_hpp */
