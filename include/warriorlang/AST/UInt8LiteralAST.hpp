//
//  UInt8LiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef UInt8LiteralAST_hpp
#define UInt8LiteralAST_hpp

#include "ExpressionAST.hpp"
#include <cstdint>

namespace warriorlang {
    /**
     Represents UInt8 literal in WarriorLang:

     ```warriorlang
     let short: UInt8 = 127; // UInt8 literal
     print(256 as UInt8); // UInt8 literal
     ```
     */
    class UInt8LiteralAST: public ExpressionAST {
    private:
        uint_fast8_t value;
    public:
        ~UInt8LiteralAST();
        UInt8LiteralAST(uint_fast8_t value): value(value) {}
    };
}
#endif /* UInt8LiteralAST_hpp */
