//
//  Lexer.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include "TokenCategory.hpp"
#include <string>
#include <vector>
#include <fstream>

namespace warriorlang {
    enum LexerStateType {
        TOKENIZER_STATE_START,
        TOKENIZER_STATE_SYMBOL,
        TOKENIZER_STATE_IDENTIFIER,
        TOKENIZER_STATE_NUMBER_LITERAL,
        TOKENIZER_STATE_NUMBER_LITERAL_ZERO,
        TOKENIZER_STATE_NUMBER_LITERAL_EXPONENTIAL,
        TOKENIZER_STATE_COMPILER_DIRECTIVE,
        TOKENIZER_STATE_DOT,
        TOKENIZER_STATE_DASH,
        TOKENIZER_STATE_SLASH,
        TOKENIZER_STATE_STRING_LITERAL,
        TOKENIZER_STATE_STRING_LITERAL_ESCAPE,
        TOKENIZER_STATE_STRING_LITERAL_INTERPOLATION,
        TOKENIZER_STATE_CHARACTER_LITERAL,
        TOKENIZER_STATE_CHARACTER_LITERAL_ESCAPE,
        TOKENIZER_STATE_INLINE_COMMENT,
        TOKENIZER_STATE_BLOCK_COMMENT,
        TOKENIZER_STATE_OPEN_PARENTHESIS
    };

    struct LexerState {
        LexerStateType type;
    };

    struct Token {
        TokenCategory category;
        std::string value;
        std::string file;
        long int startIndex;
        long int endIndex;
        long int startLine;
        long int startColumn;

        int numberRadix; // 2, 8, 10, 16
        bool numberFloatingPoint; // [digit]\.[digit]

        bool stringHasInterpolation; // \(expression)
    };

    struct Keyword {
        std::string keyword;
        TokenCategory category;
    };

    class Lexer {
        private:
            std::string file;
            char currentCharacter;
            long int currentIndex;
            long int currentLine;
            long int currentColumn;

            std::vector<Token>* tokens;
            std::vector<LexerState> states;

            std::ifstream* inputFileStream;

            long int tokenStartIndex;
            long int tokenEndIndex;
            long int tokenStartLine;
            long int tokenStartColumn;
            std::string currentSelection;
            int numberRadix;
            bool numberFloatingPoint;
            bool stringHasInterpolation;

            void logError(const std::string &message);

            void tokenStart(const LexerStateType &type);
            void tokenStartWithoutAddingCurrentCharacter();
            void tokenStartWithoutAddingCurrentCharacter(const LexerStateType &type);
            void appendToToken(const char &c);
            void appendToToken();
            void tokenEnd(const TokenCategory &category);

            void appendSingleCharacterToken(const char &c, const TokenCategory &category);
            void appendSingleCharacterToken(const TokenCategory &category);

            bool tryToReadNextCharacter();
            void appendEndOfFileToken();
            const Token* peekToken();

            void enterState(const LexerStateType &type);
            void switchState(const LexerStateType &type);
            LexerState* getState();
            void leaveState();

            void lexerStateStart(bool &readNextCharacter);
            void lexerStateSymbol(bool &readNextCharacter);
            void lexerStateIdentifier(bool &readNextCharacter);
            void lexerStateZero(bool &readNextCharacter);
            void lexerEndNumberToken(bool &readNextCharacter);
            void lexerStateNumber(bool &readNextCharacter);
            void lexerStateNumberExponential(bool &readNextCharacter);
            void lexerStateCompilerDirective(bool &readNextCharacter);
            void lexerStateDot(bool &readNextCharacter);
            void lexerStateDash(bool &readNextCharacter);
            void lexerStateSlash(bool &readNextCharacter);
            void lexerStateStringLiteral(bool &readNextCharacter);
            void lexerStateStringLiteralEscape(bool &readNextCharacter);
            void lexerStateCharacterLiteral(bool &readNextCharacter);
            void lexerStateCharacterLiteralEscape(bool &readNextCharacter);
            void lexerStateInlineComment(bool &readNextCharacter);
            void lexerStateBlockComment(bool &readNextCharacter);

            void parseState(const LexerState& state, bool &readNextCharacter);
        public:
            Lexer(const std::string &file);
            ~Lexer();

            void parse();
            const std::vector<Token> getTokens();
    };
}
#endif /* Lexer_hpp */