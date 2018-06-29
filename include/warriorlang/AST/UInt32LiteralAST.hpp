//
//  UInt32LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef UInt32LiteralAST_hpp
#define UInt32LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents UInt32 literal in WarriorLang:

     ```warriorlang
     let short: UInt32 = 10_000; // UInt32 literal
     print(-1_000 as UInt32); // UInt32 literal
     ```
     */
    class UInt32LiteralAST: public ExpressionAST {
    private:
        uint_fast32_t value;
    public:
        ~UInt32LiteralAST();
        UInt32LiteralAST(uint_fast32_t value): value(value) {}
    };
}
#endif /* UInt32LiteralAST_hpp */
