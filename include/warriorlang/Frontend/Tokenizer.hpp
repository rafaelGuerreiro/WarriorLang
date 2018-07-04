//
//  Tokenizer.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <string>
#include <vector>
#include <fstream>

namespace warriorlang {
    enum TokenCategory {
        TOKEN_EOF,
        TOKEN_SPACE, // Used to separate tokens that don't necessarily correlate

        // Types
        TOKEN_DECLARATION_CLASS, // class
        TOKEN_DECLARATION_INTERFACE, // interface
        TOKEN_DECLARATION_ENUM, // enum
        TOKEN_DECLARATION_ANNOTATION, // annotation
        TOKEN_DECLARATION_MODULE, // module         // Modules can be used to automatically insert common methods.
        TOKEN_DECLARATION_EXTENSION, // extension
        TOKEN_DECLARATION_OPERATOR, // operator

        TOKEN_DECLARATION_VAR, // var
        TOKEN_DECLARATION_LET, // let
        TOKEN_DECLARATION_GET, // get
        TOKEN_DECLARATION_SET, // set
        TOKEN_DECLARATION_FUNCTION, // function

        TOKEN_DECLARATION_INIT, // init           // Init methods can't read from self. Developers should use postconstruct methods.
        // TOKEN_DECLARATION_INIT_OPTIONAL, // init?          // Init method that can return null.
        TOKEN_DECLARATION_DEINIT, // deinit         // When this object is going to be removed from memory, this method is called.
        TOKEN_DECLARATION_POSTCONSTRUCT, // postconstruct  // A private method that is called after the complete initialization of the object. Child first.

        TOKEN_DECLARATION_INCLUDE, // include        // used to include a module in this class and all implementations.
        TOKEN_DECLARATION_TYPEALIAS, // typealias

        // Modifiers
        TOKEN_DECLARATION_PUBLIC, // public,
        TOKEN_DECLARATION_INTERNAL, // internal,       // internal allows access to the same module
        TOKEN_DECLARATION_PRIVATE, // private,        // private allows access to extensions and inheritance
        TOKEN_DECLARATION_PUBLICSET, // publicset,
        TOKEN_DECLARATION_INTERNALSET, // internalset,    // internalset allows access to the same module
        TOKEN_DECLARATION_PRIVATESET, // privateset,     // privateset allows access to extensions and inheritance
    
        TOKEN_DECLARATION_OVERRIDABLE, // overridable,    // all classes and functions are final. Use overridable to mark them as overridable.
        TOKEN_DECLARATION_OVERRIDE, // override,       // when overriding a super function
        TOKEN_DECLARATION_REQUIRED, // required,       // Forces sub-implementation to override this method. Implies it's also overridable. Therefore, it can't call any self mutating methods.
        TOKEN_DECLARATION_MUTATING, // mutating,       // mutating functions allows this function to modify the underlying object

        TOKEN_MODIFIER_MUTABLE, // mutable,        // mutable returns allows the returned instance to be assigned to `var` and be modified.
        TOKEN_MODIFIER_INOUT, // inout,          // Makes an argument mutable, which also reflects on the caller instance. `let` variables can't be passed to inout.
        TOKEN_MODIFIER_WHERE, // where,          // Used to refine generics in functions and extensions.
        TOKEN_MODIFIER_THROWS, // throws,
        TOKEN_MODIFIER_ASYNC, // async,
        TOKEN_MODIFIER_DEFAULT, // default,

        // Expressions
        TOKEN_EXPRESSION_NULL, // null,           // Same as Optional<T>.none
        TOKEN_EXPRESSION_SELF, // self,
        TOKEN_EXPRESSION_SUPER, // super,
        TOKEN_EXPRESSION_TYPE, // type,           // Like Swift's Self
        TOKEN_EXPRESSION_INCLUDES, // includes,       // check if this instance or class includes this module.
        TOKEN_EXPRESSION_NEW, // new,            // new Class(argument: "test") -> syntax-sugar to Class.init(argument: "test");
        TOKEN_EXPRESSION_THROW, // throw,
        TOKEN_EXPRESSION_TRY, // try,
        // TOKEN_EXPRESSION_TRY_OPTIONAL, // try?,
        TOKEN_EXPRESSION_CATCH, // catch,
        TOKEN_EXPRESSION_FINALLY, // finally,        // Is garanteed to be invoked, even when errors happened. This block can't return, but it can throw.
        TOKEN_EXPRESSION_DEFER, // defer,          // Is garanteed to be invoked, even when errors happened. This block can't return, but it can throw.
        TOKEN_EXPRESSION_AS, // as,
        // TOKEN_EXPRESSION_AS_OPTIONAL, // as?,
        TOKEN_EXPRESSION_IS, // is,             // same as instanceof in Java: return true if variable is String;
        TOKEN_EXPRESSION_AWAIT, // await,
        TOKEN_EXPRESSION_IF, // if, // if let, // if var,
        // TOKEN_EXPRESSION_ELSE_IF, // else if,
        TOKEN_EXPRESSION_ELSE, // else,
        TOKEN_EXPRESSION_UNLESS, // unless,
        TOKEN_EXPRESSION_GUARD, // guard, // guard let, // guard var,
        TOKEN_EXPRESSION_WHILE, // while,
        TOKEN_EXPRESSION_FOR, // for,
        TOKEN_EXPRESSION_IN, // in,
        TOKEN_EXPRESSION_DO, // do,
        TOKEN_EXPRESSION_BREAK, // break,
        TOKEN_EXPRESSION_CONTINUE, // continue,
        TOKEN_EXPRESSION_RETURN, // return

        // Compile directives
        TOKEN_COMPILE_DIRECTIVE_IF, // #if
        TOKEN_COMPILE_DIRECTIVE_ELSE, // #else
        TOKEN_COMPILE_DIRECTIVE_ELSEIF, // #elseif
        TOKEN_COMPILE_DIRECTIVE_ENDIF, // #endif
        TOKEN_COMPILE_DIRECTIVE_LINE, // #line
        TOKEN_COMPILE_DIRECTIVE_FUNCTION, // #function
        TOKEN_COMPILE_DIRECTIVE_ERROR, // #error
        TOKEN_COMPILE_DIRECTIVE_WARNING, // #warning

        // Literals

        /* 
          [0-9][0-9_]*
          0b[0-1][0-1_]*
          0o[0-7][0-7_]*
          0x[0-9a-fA-F][0-9a-fA-F_]*
        */
        TOKEN_LITERAL_INTEGER,

        /* 
          [0-9][0-9_]*\.[0-9_]+
          [0-9][0-9_]*\.?[0-9_]*[eE][+-]?[0-9_]+
        */
        TOKEN_LITERAL_FLOAT,
        /*
          Strings are double-quoted.
          "text \"escaping using \\ and \"."
          - It allows common escaping:
             \"  // " character
             \\  // \ character
             \n  // new line
             \t  // tab
             \$  // $ character that escapes the interpolation
             \b  // backspace
             \r  // carriage return
             \f  // formfeed

          - It allows interpolation:
             "regular string"
             "regular string, value: ${singleNamedVariable}"
             "regular string, value: ${1 + 2 / 4 * 7}"
         */
        TOKEN_LITERAL_STRING,

        /*
          Characters are single quoted and single value.

          var char: Character = 'a';

          - It allows common escaping:
             \'  // ' character
             \\  // \ character
             \n  // new line
             \t  // tab
             \b  // backspace
             \r  // carriage return
             \f  // formfeed
        */
        TOKEN_LITERAL_CHARACTER,

        /*
            Block comment
        */
        TOKEN_COMMENT, // Inline comment

        TOKEN_UNDERSCORE, // _      Used to ignore variables.

        TOKEN_COMPOSED_PUNCTUATION_ARROW, // ->
        TOKEN_COMPOSED_PUNCTUATION_INTERPOLATION_START, // ${
        TOKEN_COMPOSED_PUNCTUATION_INTERPOLATION_END, // }

        TOKEN_PUNCTUATION_LEFT_PARENTHESIS, // (
        TOKEN_PUNCTUATION_RIGHT_PARENTHESIS, // )
        TOKEN_PUNCTUATION_LEFT_SQUARE_BRACKETS, // [
        TOKEN_PUNCTUATION_RIGHT_SQUARE_BRACKETS, // ]
        TOKEN_PUNCTUATION_LEFT_CURLY_BRACE, // {
        TOKEN_PUNCTUATION_RIGHT_CURLY_BRACE, // }
        TOKEN_PUNCTUATION_LEFT_ANGLE_BRACKETS, // <
        TOKEN_PUNCTUATION_RIGHT_ANGLE_BRACKETS, // >
        TOKEN_PUNCTUATION_DOT, // .
        TOKEN_PUNCTUATION_COMMA, // ,
        TOKEN_PUNCTUATION_COLON, // :
        TOKEN_PUNCTUATION_SEMICOLON, // ;
        TOKEN_PUNCTUATION_PLUS, // +
        TOKEN_PUNCTUATION_MINUS, // -
        TOKEN_PUNCTUATION_ASTERISK, // *
        TOKEN_PUNCTUATION_SLASH, // /
        TOKEN_PUNCTUATION_XOR, // ^
        TOKEN_PUNCTUATION_PIPE, // |
        TOKEN_PUNCTUATION_PERCENT, // %
        TOKEN_PUNCTUATION_TILDE, // ~
        TOKEN_PUNCTUATION_BACKTICK, // `
        TOKEN_PUNCTUATION_EQUAL, // =
        TOKEN_PUNCTUATION_AT, // @
        TOKEN_PUNCTUATION_POUND, // #
        TOKEN_PUNCTUATION_AMPERSAND, // &
        TOKEN_PUNCTUATION_BACKSLASH, // "\"
        TOKEN_PUNCTUATION_EXCLAMATION, // !
        TOKEN_PUNCTUATION_QUESTION, // ?
        TOKEN_IDENTIFIER, // [a-zA-Z_$][a-zA-Z0-9_$]*
    };

    enum TokenizerState {
        TOKENIZER_STATE_START,
        TOKENIZER_STATE_SYMBOL,
        TOKENIZER_STATE_NUMBER_LITERAL,
        TOKENIZER_STATE_NUMBER_LITERAL_ZERO,
        TOKENIZER_STATE_COMPILER_DIRECTIVE,
        TOKENIZER_STATE_DASH,
        TOKENIZER_STATE_SLASH,
        TOKENIZER_STATE_STRING_LITERAL,
        TOKENIZER_STATE_STRING_LITERAL_ESCAPE,
        TOKENIZER_STATE_CHARACTER_LITERAL,

        TOKENIZER_STATE_INLINE_COMMENT,
        TOKENIZER_STATE_BLOCK_COMMENT

        // TokenizeStateSymbol,
        // TokenizeStateZero, // "0", which might lead to "0x"
        // TokenizeStateNumber, // "123", "0x123"
        // TokenizeStateNumberDot,
        // TokenizeStateFloatFraction, // "123.456", "0x123.456"
        // TokenizeStateFloatExponentUnsigned, // "123.456e", "123e", "0x123p"
        // TokenizeStateFloatExponentNumber, // "123.456e-", "123.456e5", "123.456e5e-5"
        // TokenizeStateString,
        // TokenizeStateStringEscape,
        // TokenizeStateCharLiteral,
        // TokenizeStateCharLiteralEnd,
        // TokenizeStateSawStar,
        // TokenizeStateSawSlash,
        // TokenizeStateSawBackslash,
        // TokenizeStateSawPercent,
        // TokenizeStateSawPlus,
        // TokenizeStateSawDash,
        // TokenizeStateSawAmpersand,
        // TokenizeStateSawXor,
        // TokenizeStateSawPipe,
        // TokenizeStateLineComment,
        // TokenizeStateLineString,
        // TokenizeStateLineStringEnd,
        // TokenizeStateLineStringContinue,
        // TokenizeStateLineStringContinueC,
        // TokenizeStateSawEq,
        // TokenizeStateSawBang,
        // TokenizeStateSawLessThan,
        // TokenizeStateSawLessThanLessThan,
        // TokenizeStateSawGreaterThan,
        // TokenizeStateSawGreaterThanGreaterThan,
        // TokenizeStateSawDot,
        // TokenizeStateSawAtSign,
        // TokenizeStateCharCode,
        // TokenizeStateError,
        // TokenizeStateLBracket,
        // TokenizeStateLBracketStar,
    };

    struct NumericTokenMetadata {
        int radix; // 2, 8, 10, 16
        bool floatingPoint; // [digit]\.[digit]
    };

    struct Token {
        TokenCategory category;
        std::string value;
        std::string file;
        long int startIndex;
        long int endIndex;
        long int startLine;
        long int startColumn;

        union {
            NumericTokenMetadata numericTokenMetadata;
        };
    };

    struct Keyword {
        std::string keyword;
        TokenCategory category;
    };

    class Tokenizer {
        private:
            std::string file;
            TokenizerState state;
            char currentCharacter;
            long int currentIndex;
            long int currentLine;
            long int currentColumn;

            std::vector<Token>* tokens;
            std::ifstream* inputFileStream;

            long int tokenStartIndex;
            long int tokenEndIndex;
            long int tokenStartLine;
            long int tokenStartColumn;
            std::string currentSelection;

            NumericTokenMetadata numericTokenMetadata;

            void logError(const std::string &message);

            void tokenStart();
            void tokenStart(const TokenizerState &state);
            void tokenStartWithoutAddingCurrentCharacter();
            void tokenStartWithoutAddingCurrentCharacter(const TokenizerState &state);
            void appendToToken(const char &c);
            void appendToToken();
            void tokenEnd(const TokenCategory &category);

            void appendSingleCharacterToken(const TokenCategory &category);

            bool tryToReadNextCharacter();
            void appendSpaceTokenIfLastTokenWasNotSpaceAlready();
            void appendEndOfFileToken();
            const Token* peekToken();

            void tokenizerStateStart(bool &readNextCharacter);
            void tokenizerStateSymbol(bool &readNextCharacter);
            void tokenizerStateZero(bool &readNextCharacter);
            void tokenizerStateNumber(bool &readNextCharacter);
            void tokenizerStateCompilerDirective(bool &readNextCharacter);
            void tokenizerStateDash(bool &readNextCharacter);
            void tokenizerStateSlash(bool &readNextCharacter);
            void tokenizerStateStringLiteral(bool &readNextCharacter);
            void tokenizerStateStringLiteralEscape(bool &readNextCharacter);
            void tokenizerStateCharacterLiteral(bool &readNextCharacter);
            void tokenizerStateInlineComment(bool &readNextCharacter);
            void tokenizerStateBlockComment(bool &readNextCharacter);
        public:
            Tokenizer(const std::string &file);
            ~Tokenizer();

            void tokenize();
            const std::vector<Token> getTokens();
    };
}
#endif /* Tokenizer_hpp */