//
//  UInt16LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef UInt16LiteralAST_hpp
#define UInt16LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents UInt16 literal in WarriorLang:

     ```warriorlang
     let short: UInt16 = 10_000; // UInt16 literal
     print(-1_000 as UInt16); // UInt16 literal
     ```
     */
    class UInt16LiteralAST: public ExpressionAST {
    private:
        uint_fast16_t value;
    public:
        ~UInt16LiteralAST();
        UInt16LiteralAST(uint_fast16_t value): value(value) {}
    };
}
#endif /* UInt16LiteralAST_hpp */
