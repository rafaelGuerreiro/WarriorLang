//
//  Tokenizer.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Frontend/Tokenizer.hpp"
#include <vector>

namespace warriorlang {
    enum TokenizeState {
        TokenizeStateStart,
        TokenizeStateSymbol,
        TokenizeStateZero, // "0", which might lead to "0x"
        TokenizeStateNumber, // "123", "0x123"
        TokenizeStateNumberDot,
        TokenizeStateFloatFraction, // "123.456", "0x123.456"
        TokenizeStateFloatExponentUnsigned, // "123.456e", "123e", "0x123p"
        TokenizeStateFloatExponentNumber, // "123.456e-", "123.456e5", "123.456e5e-5"
        TokenizeStateString,
        TokenizeStateStringEscape,
        TokenizeStateCharLiteral,
        TokenizeStateCharLiteralEnd,
        TokenizeStateSawStar,
        TokenizeStateSawSlash,
        TokenizeStateSawBackslash,
        TokenizeStateSawPercent,
        TokenizeStateSawPlus,
        TokenizeStateSawDash,
        TokenizeStateSawAmpersand,
        TokenizeStateSawXor,
        TokenizeStateSawPipe,
        TokenizeStateLineComment,
        TokenizeStateLineString,
        TokenizeStateLineStringEnd,
        TokenizeStateLineStringContinue,
        TokenizeStateLineStringContinueC,
        TokenizeStateSawEq,
        TokenizeStateSawBang,
        TokenizeStateSawLessThan,
        TokenizeStateSawLessThanLessThan,
        TokenizeStateSawGreaterThan,
        TokenizeStateSawGreaterThanGreaterThan,
        TokenizeStateSawDot,
        TokenizeStateSawAtSign,
        TokenizeStateCharCode,
        TokenizeStateError,
        TokenizeStateLBracket,
        TokenizeStateLBracketStar,
    };

    struct Tokenize {
        // Buf *buf;
        long int position;
        TokenizeState state;
        std::vector<Token> *tokens;
        long int line;
        int column;
        Token *currentToken;
    };
}
