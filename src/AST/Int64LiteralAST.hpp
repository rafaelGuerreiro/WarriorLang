//
//  Int64LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef Int64LiteralAST_hpp
#define Int64LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents Int64 literal in WarriorLang:

     ```warriorlang
     let short: Int64 = 10_000; // Int64 literal
     print(-1_000); // Int64 literal
     ```
     */
    class Int64LiteralAST: public ExpressionAST {
    private:
        int_fast64_t value;
    public:
        ~Int64LiteralAST();
        Int64LiteralAST(int_fast64_t value): value(value) {}
    };
}
#endif /* Int64LiteralAST_hpp */
