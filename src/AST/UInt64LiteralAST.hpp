//
//  UInt64LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef UInt64LiteralAST_hpp
#define UInt64LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents UInt64 literal in WarriorLang:

     ```warriorlang
     let short: UInt64 = 10_000; // UInt64 literal
     print(-1_000); // UInt64 literal
     ```
     */
    class UInt64LiteralAST: public ExpressionAST {
    private:
        uint_fast64_t value;
    public:
        ~UInt64LiteralAST();
        UInt64LiteralAST(uint_fast64_t value): value(value) {}
    };
}
#endif /* UInt64LiteralAST_hpp */
