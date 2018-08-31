//
//  Lexer.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Parser/Lexer.hpp"
#include "warriorlang/utils.hpp"
#include <iostream>
#include <sstream>

namespace warriorlang {
    static const std::vector<Keyword> keywords = std::vector<Keyword> {
        { "class",          TOKEN_DECLARATION_CLASS },
        { "interface",      TOKEN_DECLARATION_INTERFACE },
        { "enum",           TOKEN_DECLARATION_ENUM },
        { "annotation",     TOKEN_DECLARATION_ANNOTATION },
        { "module",         TOKEN_DECLARATION_MODULE },
        { "extension",      TOKEN_DECLARATION_EXTENSION },
        { "operator",       TOKEN_DECLARATION_OPERATOR },
        { "higherThan",     TOKEN_DECLARATION_OPERATOR_HIGHER_THAN },
        { "lowerThan",      TOKEN_DECLARATION_OPERATOR_LOWER_THAN },
        { "infix",          TOKEN_DECLARATION_OPERATOR_INFIX },
        { "prefix",         TOKEN_DECLARATION_OPERATOR_PREFIX },
        { "postfix",        TOKEN_DECLARATION_OPERATOR_POSTFIX },
        { "associativity",  TOKEN_DECLARATION_OPERATOR_ASSOCIATIVITTY },
        { "left",           TOKEN_DECLARATION_OPERATOR_ASSOCIATIVITTY_LEFT },
        { "right",          TOKEN_DECLARATION_OPERATOR_ASSOCIATIVITTY_RIGHT },
        { "none",           TOKEN_DECLARATION_OPERATOR_ASSOCIATIVITTY_NONE },
        { "var",            TOKEN_DECLARATION_VAR },
        { "let",            TOKEN_DECLARATION_LET },
        { "get",            TOKEN_DECLARATION_GET },
        { "set",            TOKEN_DECLARATION_SET },
        { "function",       TOKEN_DECLARATION_FUNCTION },
        { "init",           TOKEN_DECLARATION_INIT },
        // { "init?",          TOKEN_DECLARATION_INIT_OPTIONAL },
        { "deinit",         TOKEN_DECLARATION_DEINIT },
        { "lazy",           TOKEN_DECLARATION_LAZY },
        { "postconstruct",  TOKEN_DECLARATION_POSTCONSTRUCT },
        { "include",        TOKEN_DECLARATION_INCLUDE },
        { "typealias",      TOKEN_DECLARATION_TYPEALIAS },
        { "public",         TOKEN_DECLARATION_PUBLIC },
        { "internal",       TOKEN_DECLARATION_INTERNAL },
        { "private",        TOKEN_DECLARATION_PRIVATE },
        { "publicset",      TOKEN_DECLARATION_PUBLICSET },
        { "internalset",    TOKEN_DECLARATION_INTERNALSET },
        { "privateset",     TOKEN_DECLARATION_PRIVATESET },
        { "overridable",    TOKEN_DECLARATION_OVERRIDABLE },
        { "override",       TOKEN_DECLARATION_OVERRIDE },
        { "required",       TOKEN_DECLARATION_REQUIRED },
        { "mutating",       TOKEN_DECLARATION_MUTATING },
        // { "mutable",        TOKEN_MODIFIER_MUTABLE },
        { "inout",          TOKEN_MODIFIER_INOUT },
        { "where",          TOKEN_MODIFIER_WHERE },
        { "throws",         TOKEN_MODIFIER_THROWS },
        { "rethrows",       TOKEN_MODIFIER_RETHROWS },
        { "async",          TOKEN_MODIFIER_ASYNC },
        { "default",        TOKEN_MODIFIER_DEFAULT },
        { "null",           TOKEN_EXPRESSION_NULL },
        { "self",           TOKEN_EXPRESSION_SELF },
        { "super",          TOKEN_EXPRESSION_SUPER },
        { "type",           TOKEN_EXPRESSION_TYPE },
        { "includes",       TOKEN_EXPRESSION_INCLUDES },
        { "new",            TOKEN_EXPRESSION_NEW },
        { "throw",          TOKEN_EXPRESSION_THROW },
        { "try",            TOKEN_EXPRESSION_TRY },
        // { "try?",           TOKEN_EXPRESSION_TRY_OPTIONAL },
        { "catch",          TOKEN_EXPRESSION_CATCH },
        { "finally",        TOKEN_EXPRESSION_FINALLY },
        { "defer",          TOKEN_EXPRESSION_DEFER },
        { "as",             TOKEN_EXPRESSION_AS },
        // { "as?",            TOKEN_EXPRESSION_AS_OPTIONAL },
        { "is",             TOKEN_EXPRESSION_IS },
        { "await",          TOKEN_EXPRESSION_AWAIT },
        { "if",             TOKEN_EXPRESSION_IF },
        // { "else if",        TOKEN_EXPRESSION_ELSE_IF },
        { "else",           TOKEN_EXPRESSION_ELSE },
        { "unless",         TOKEN_EXPRESSION_UNLESS },
        { "guard",          TOKEN_EXPRESSION_GUARD },
        { "while",          TOKEN_EXPRESSION_WHILE },
        { "for",            TOKEN_EXPRESSION_FOR },
        { "in",             TOKEN_EXPRESSION_IN },
        { "do",             TOKEN_EXPRESSION_DO },
        { "break",          TOKEN_EXPRESSION_BREAK },
        { "continue",       TOKEN_EXPRESSION_CONTINUE },
        { "return",         TOKEN_EXPRESSION_RETURN },
        { "_",              TOKEN_UNDERSCORE }
    };

    static const std::vector<Keyword> compilerDirectives = std::vector<Keyword> {
        { "#if",            TOKEN_COMPILE_DIRECTIVE_IF },
        { "#else",          TOKEN_COMPILE_DIRECTIVE_ELSE },
        { "#elseif",        TOKEN_COMPILE_DIRECTIVE_ELSEIF },
        { "#endif",         TOKEN_COMPILE_DIRECTIVE_ENDIF },
        { "#line",          TOKEN_COMPILE_DIRECTIVE_LINE },
        { "#function",      TOKEN_COMPILE_DIRECTIVE_FUNCTION },
        { "#error",         TOKEN_COMPILE_DIRECTIVE_ERROR },
        { "#warning",       TOKEN_COMPILE_DIRECTIVE_WARNING }
    };

    static std::string nameOf(const LexerState &state) {
        switch(state.type) {
        case TOKENIZER_STATE_START: return "TOKENIZER_STATE_START";
        case TOKENIZER_STATE_SYMBOL: return "TOKENIZER_STATE_SYMBOL";
        case TOKENIZER_STATE_IDENTIFIER: return "TOKENIZER_STATE_IDENTIFIER";
        case TOKENIZER_STATE_NUMBER_LITERAL: return "TOKENIZER_STATE_NUMBER_LITERAL";
        case TOKENIZER_STATE_NUMBER_LITERAL_ZERO: return "TOKENIZER_STATE_NUMBER_LITERAL_ZERO";
        case TOKENIZER_STATE_NUMBER_LITERAL_EXPONENTIAL: return "TOKENIZER_STATE_NUMBER_LITERAL_EXPONENTIAL";
        case TOKENIZER_STATE_COMPILER_DIRECTIVE: return "TOKENIZER_STATE_COMPILER_DIRECTIVE";
        case TOKENIZER_STATE_DASH: return "TOKENIZER_STATE_DASH";
        case TOKENIZER_STATE_SLASH: return "TOKENIZER_STATE_SLASH";
        case TOKENIZER_STATE_STRING_LITERAL: return "TOKENIZER_STATE_STRING_LITERAL";
        case TOKENIZER_STATE_STRING_LITERAL_ESCAPE: return "TOKENIZER_STATE_STRING_LITERAL_ESCAPE";
        case TOKENIZER_STATE_STRING_LITERAL_INTERPOLATION: return "TOKENIZER_STATE_STRING_LITERAL_INTERPOLATION";
        case TOKENIZER_STATE_CHARACTER_LITERAL: return "TOKENIZER_STATE_CHARACTER_LITERAL";
        case TOKENIZER_STATE_CHARACTER_LITERAL_ESCAPE: return "TOKENIZER_STATE_CHARACTER_LITERAL_ESCAPE";
        case TOKENIZER_STATE_INLINE_COMMENT: return "TOKENIZER_STATE_INLINE_COMMENT";
        case TOKENIZER_STATE_BLOCK_COMMENT: return "TOKENIZER_STATE_BLOCK_COMMENT";
        case TOKENIZER_STATE_OPEN_PARENTHESIS: return "TOKENIZER_STATE_OPEN_PARENTHESIS";
        }
        return "(unknown state)";
    }

    static const TokenCategory* getCategory(const std::string &word, const std::vector<Keyword> &list) {
        for (unsigned long int index = 0; index < list.size(); index++)
            if (word == list[index].keyword)
                return &list[index].category;

        return nullptr;
    }

    static const TokenCategory* getKeywordCategory(const std::string &word) {
        return getCategory(word, keywords);
    }

    static const TokenCategory* getCompilerDirectiveCategory(const std::string &word) {
        return getCategory(word, compilerDirectives);
    }

    static bool isBinaryDigit(const char &c) {
        return (c >= '0' && c <= '1');
    }

    static bool isOctalDigit(const char &c) {
        return (c >= '0' && c <= '7');
    }

    static bool isHexdecimalDigit(const char &c) {
        return isdigit(c) ||
                (c >= 'a' && c <= 'f') ||
                (c >= 'A' && c <= 'F');
    }

    static bool isValidDigit(const int &radix, const char &c) {
        if (radix == 2)
            return isBinaryDigit(c);

        if (radix == 8)
            return isOctalDigit(c);

        if (radix == 10)
            return isdigit(c);

        if (radix == 16)
            return isHexdecimalDigit(c);

        return false;
    }

    static bool isExponentialIdentifier(const int &radix, const char &c) {
        if (radix == 10)
            return c == 'e' || c == 'E';

        if (radix == 16)
            return c == 'p' || c == 'P';

        return false;
    }

    Lexer::Lexer(const std::string &file) {
        this->file = file;

        this->states = std::vector<LexerState> {
            LexerState {
            /* type: */ TOKENIZER_STATE_START
            }
        };

        this->currentCharacter = '\0';
        this->currentIndex = 0;
        this->currentLine = 1;
        this->currentColumn = 1;

        this->tokens = new std::vector<Token>();
        this->inputFileStream = new std::ifstream();

        this->tokenStartIndex = 0;
        this->tokenEndIndex = 0;
        this->tokenStartLine = 0;
        this->tokenStartColumn = 0;
        this->currentSelection = "";
        this->numberRadix = 0;
        this->numberFloatingPoint = false;
        this->stringHasInterpolation = false;
    }

    Lexer::~Lexer() {
        if (this->tokens != nullptr) {
            this->tokens->clear();
            safelyDeletePointer(this->tokens);
        }

        if (this->inputFileStream != nullptr && this->inputFileStream->is_open()) {
            this->inputFileStream->close();
            safelyDeletePointer(this->inputFileStream);
        }
    }

    static void printStates(const std::string &text, const std::vector<LexerState> &states) {
        std::cout << text << "[ ";
        for (auto state : states)
            std::cout << nameOf(state) << ' ';
        std::cout << "]\n";
    }

    void Lexer::enterState(const LexerStateType &type) {
        states.push_back(LexerState {
            /*type: */type
        });
        printStates("Enter state: ", this->states);
    }

    void Lexer::switchState(const LexerStateType &type) {
        this->leaveState();
        this->enterState(type);
    }

    LexerState* Lexer::getState() {
        if (states.empty())
            return nullptr;

        return &(states[states.size() - 1]);
    }

    void Lexer::leaveState() {
        if (!states.empty())
            states.pop_back();

        printStates("Leave state: ", this->states);
    }

    void Lexer::logError(const std::string &message) {
        std::cout << this->file
                  << ':'
                  << this->currentLine
                  << ':'
                  << this->currentColumn
                  << ": error: "
                  << message
                  << '\n';
    }

    const std::vector<Token> Lexer::getTokens() {
        return *this->tokens;
    }

    void Lexer::tokenStart(const LexerStateType &type) {
        this->tokenStartWithoutAddingCurrentCharacter();
        this->currentSelection = std::string(1, this->currentCharacter);
        this->enterState(type);
    }

    void Lexer::tokenStartWithoutAddingCurrentCharacter() {
        this->tokenStartIndex = this->currentIndex;
        this->tokenEndIndex = this->currentIndex + 1;
        this->tokenStartLine = this->currentLine;
        this->tokenStartColumn = this->currentColumn;
        this->currentSelection = "";
        this->numberRadix = 0;
        this->numberFloatingPoint = false;
        this->stringHasInterpolation = false;
    }

    void Lexer::tokenStartWithoutAddingCurrentCharacter(const LexerStateType &type) {
        this->tokenStartWithoutAddingCurrentCharacter();
        this->enterState(type);
    }

    void Lexer::appendToToken(const char &c) {
        this->tokenEndIndex += 1;
        this->currentSelection += c;
    }

    void Lexer::appendToToken() {
        this->appendToToken(this->currentCharacter);
    }

    void Lexer::tokenEnd(const TokenCategory &category) {
        this->leaveState();

        int radix = this->numberRadix;
        bool floatingPoint = this->numberFloatingPoint;
        if (category != TOKEN_LITERAL_FLOAT && category != TOKEN_LITERAL_INTEGER) {
            radix = 0;
            floatingPoint = false;
        }

        // bool stringHasInterpolation = this->stringHasInterpolation;
        // if (category != TOKEN_LITERAL_STRING)
        //     stringHasInterpolation = false;

        this->tokens->push_back(Token {
            /* category: */                 category,
            /* value: */                    this->currentSelection,
            /* file: */                     this->file,
            /* startIndex: */               this->tokenStartIndex,
            /* endIndex: */                 this->tokenEndIndex,
            /* startLine: */                this->tokenStartLine,
            /* startColumn: */              this->tokenStartColumn,
            /* numberRadix: */              radix,
            /* numberFloatingPoint: */      floatingPoint,
            /* stringHasInterpolation: */   false
        });

        this->currentSelection = "";
        this->numberRadix = 0;
        this->numberFloatingPoint = false;
        this->stringHasInterpolation = false;
    }

    void Lexer::appendSingleCharacterToken(const char &c, const TokenCategory &category) {
        this->tokenStartWithoutAddingCurrentCharacter();
        this->currentSelection += c;
        this->tokenEnd(category);
    }

    void Lexer::appendSingleCharacterToken(const TokenCategory &category) {
        this->tokenStart(TOKENIZER_STATE_SYMBOL);
        this->tokenEnd(category);
    }

    void Lexer::appendEndOfFileToken() {
        const Token* lastToken = peekToken();
        if (lastToken != nullptr &&
            lastToken->category == TOKEN_EOF) // Nothing can be added after an EOF.
            return;

        this->tokens->push_back(Token {
            /* category: */                 TOKEN_EOF,
            /* value: */                    "",
            /* file: */                     this->file,
            /* startIndex: */               this->currentIndex,
            /* endIndex: */                 this->currentIndex + 1,
            /* startLine: */                this->currentLine,
            /* startColumn: */              this->currentColumn,
            /* numberRadix: */              0,
            /* numberFloatingPoint: */      false,
            /* stringHasInterpolation: */   false
        });
    }

    bool Lexer::tryToReadNextCharacter() {
        this->currentCharacter = this->inputFileStream->get();
        return this->inputFileStream->good();
    }

    const Token* Lexer::peekToken() {
        if (this->tokens->empty())
            return nullptr;

        return &((*this->tokens)[this->tokens->size() - 1]);
    }

    void Lexer::lexerStateStart(bool &readNextCharacter) {
        readNextCharacter = true;
        if (isspace(this->currentCharacter)) {
            const Token* lastToken = this->peekToken();
            if (lastToken != nullptr && lastToken->category != TOKEN_SPACE)
                this->appendSingleCharacterToken(TOKEN_SPACE);

            return;
        } else if (this->currentCharacter == '0')
            this->tokenStart(TOKENIZER_STATE_NUMBER_LITERAL_ZERO);
        else if (isdigit(this->currentCharacter))
            this->tokenStart(TOKENIZER_STATE_NUMBER_LITERAL);
        else if (isalpha(this->currentCharacter) ||
                this->currentCharacter == '_' ||
                this->currentCharacter == '$')
            this->tokenStart(TOKENIZER_STATE_SYMBOL);
        else if (this->currentCharacter == '"')
            this->tokenStartWithoutAddingCurrentCharacter(TOKENIZER_STATE_STRING_LITERAL);
        else if (this->currentCharacter == '\'')
            this->tokenStartWithoutAddingCurrentCharacter(TOKENIZER_STATE_CHARACTER_LITERAL);
        else if (this->currentCharacter == '/')
            this->tokenStart(TOKENIZER_STATE_SLASH);
        else if (this->currentCharacter == '-')
            this->tokenStart(TOKENIZER_STATE_DASH);
        else if (this->currentCharacter == '#')
            this->tokenStart(TOKENIZER_STATE_COMPILER_DIRECTIVE);
        else if (this->currentCharacter == '`')
            this->tokenStartWithoutAddingCurrentCharacter(TOKENIZER_STATE_IDENTIFIER);
        else if (this->currentCharacter == '(') {
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_PARENTHESIS);
            this->enterState(TOKENIZER_STATE_OPEN_PARENTHESIS);
        } else if (this->currentCharacter == ')') {
            const LexerState *state = this->getState();

            this->leaveState();
            if (state == nullptr || state->type == TOKENIZER_STATE_OPEN_PARENTHESIS)
                this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_PARENTHESIS);
        } else if (this->currentCharacter == '.') {
            this->tokenStart(TOKENIZER_STATE_DOT);
        }

        // Single char tokens
        else if (this->currentCharacter == '{')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_CURLY_BRACE);
        else if (this->currentCharacter == '}')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_CURLY_BRACE);
        else if (this->currentCharacter == '[')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_SQUARE_BRACKETS);
        else if (this->currentCharacter == ']')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_SQUARE_BRACKETS);
        else if (this->currentCharacter == '<')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_ANGLE_BRACKETS);
        else if (this->currentCharacter == '>')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_ANGLE_BRACKETS);
        else if (this->currentCharacter == ',')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_COMMA);
        else if (this->currentCharacter == ':')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_COLON);
        else if (this->currentCharacter == ';')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_SEMICOLON);
        else if (this->currentCharacter == '+')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_PLUS);
        else if (this->currentCharacter == '*')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_ASTERISK);
        else if (this->currentCharacter == '^')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_XOR);
        else if (this->currentCharacter == '|')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_PIPE);
        else if (this->currentCharacter == '%')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_PERCENT);
        else if (this->currentCharacter == '~')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_TILDE);
        else if (this->currentCharacter == '=')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_EQUAL);
        else if (this->currentCharacter == '@')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_AT);
        else if (this->currentCharacter == '&')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_AMPERSAND);
        else if (this->currentCharacter == '\\')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_BACKSLASH);
        else if (this->currentCharacter == '!')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_EXCLAMATION);
        else if (this->currentCharacter == '?')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_QUESTION);
        else {
            std::stringstream ss;
            ss << "Unknown character: " << this->currentCharacter;
            logError(ss.str());
        }
    }

    void Lexer::lexerStateSymbol(bool &readNextCharacter) {
        // First char is alpha.
        if (isalpha(this->currentCharacter) ||
                isdigit(this->currentCharacter) ||
                this->currentCharacter == '_' ||
                this->currentCharacter == '$')
            this->appendToToken();
        else {
            const TokenCategory* category = getKeywordCategory(this->currentSelection);

            if (category == nullptr)
                this->tokenEnd(TOKEN_IDENTIFIER);
            else
                this->tokenEnd(*category);
            readNextCharacter = false;
        }
    }

    void Lexer::lexerStateIdentifier(bool &readNextCharacter) {
        // First char is `.
        readNextCharacter = true;
        if (isalpha(this->currentCharacter) ||
                this->currentCharacter == '_' ||
                this->currentCharacter == '$')
            this->appendToToken();
        else if (this->currentSelection.size() > 0 &&
                isdigit(this->currentCharacter))
            this->appendToToken();
        else if (this->currentCharacter == '`')
            this->tokenEnd(TOKEN_IDENTIFIER);
        else {
            logError("Invalid token");
        }
    }

    void Lexer::lexerEndNumberToken(bool &readNextCharacter) {
        TokenCategory category = this->numberFloatingPoint ?
                                    TOKEN_LITERAL_FLOAT :
                                    TOKEN_LITERAL_INTEGER;

        this->tokenEnd(category);
        readNextCharacter = false;

        this->numberRadix = 0;
        this->numberFloatingPoint = false;
    }

    void Lexer::lexerStateNumber(bool &readNextCharacter) {
        // First char is a digit.
        if (this->numberRadix == 0)
            this->numberRadix = 10;

        readNextCharacter = true;

        if (this->currentCharacter == '_')
            this->appendToToken();
        else if (!this->numberFloatingPoint &&
               this->currentCharacter == '.') {

            this->tryToReadNextCharacter();

            if (isdigit(this->currentCharacter) ||
                    this->currentCharacter == '_') {
                this->numberFloatingPoint = true;
                this->appendToToken('.');
                this->appendToToken(this->currentCharacter);
                readNextCharacter = true;
            } else {
                this->lexerEndNumberToken(readNextCharacter);
                readNextCharacter = false;
                this->tokenStartWithoutAddingCurrentCharacter(TOKENIZER_STATE_DOT);
                this->appendToToken('.');
            }
        }
        else if (isValidDigit(this->numberRadix, this->currentCharacter))
            this->appendToToken();
        else if (isExponentialIdentifier(this->numberRadix, this->currentCharacter)) {
            this->appendToToken();
            this->numberFloatingPoint = true;
            this->enterState(TOKENIZER_STATE_NUMBER_LITERAL_EXPONENTIAL);
        } else {
            this->lexerEndNumberToken(readNextCharacter);
        }
    }

    void Lexer::lexerStateNumberExponential(bool &readNextCharacter) {
        // last digit was e or p.
        readNextCharacter = true;
        if (isValidDigit(this->numberRadix, this->currentCharacter) ||
                this->currentCharacter == '+' ||
                this->currentCharacter == '-') {
            this->appendToToken();
            this->leaveState();
        } else {
            this->lexerEndNumberToken(readNextCharacter);
        }
    }

    void Lexer::lexerStateZero(bool &readNextCharacter) {
        // First char is 0.
        readNextCharacter = true;
        this->switchState(TOKENIZER_STATE_NUMBER_LITERAL);

        if(this->currentCharacter == 'x') {
            this->numberRadix = 16;
            this->appendToToken();
        } else if (this->currentCharacter == 'o') {
            this->numberRadix = 8;
            this->appendToToken();
        } else if (this->currentCharacter == 'b') {
            this->numberRadix = 2;
            this->appendToToken();
        } else if (!this->numberFloatingPoint &&
                this->currentCharacter == '.') {
            this->numberFloatingPoint = true;
            this->appendToToken();
        } else if (this->currentCharacter == '_' ||
                isdigit(this->currentCharacter)) {
            this->appendToToken();
        } else {
            this->numberRadix = 10;
            this->numberFloatingPoint = false;
            this->tokenEnd(TOKEN_LITERAL_INTEGER);
            readNextCharacter = false;

            this->numberRadix = 0;
            this->numberFloatingPoint = false;
        }
    }

    void Lexer::lexerStateCompilerDirective(bool &readNextCharacter) {
        // First char is #
        readNextCharacter = true;
        if (isalpha(this->currentCharacter))
            this->appendToToken();
        else {
            const TokenCategory* category = getCompilerDirectiveCategory(this->currentSelection);
            if (category == nullptr)
                if (this->currentSelection == "#")
                    this->tokenEnd(TOKEN_PUNCTUATION_POUND);
                else
                    this->tokenEnd(TOKEN_IDENTIFIER);
            else
                this->tokenEnd(*category);
            readNextCharacter = false;
        }
    }

    void Lexer::lexerStateDot(bool &readNextCharacter) {
        // First char is . not part of a number.

        // ...
        // ..<
        // .method | .variable

        readNextCharacter = true;
        if (this->currentCharacter == '.' && (
                    this->currentSelection == "."  ||
                    this->currentSelection == ".."
                ))
        {
            this->appendToToken();
            if (this->currentSelection == "...")
                this->tokenEnd(TOKEN_COMPOSED_CLOSED_RANGE);
        } else if (this->currentCharacter == '<' && this->currentSelection == "..") {
            this->appendToToken();
            if (this->currentSelection == "..<")
                this->tokenEnd(TOKEN_COMPOSED_HALF_OPEN_RANGE);
        } else {
            this->tokenEnd(TOKEN_PUNCTUATION_DOT);
            readNextCharacter = false;
        }
    }

    void Lexer::lexerStateDash(bool &readNextCharacter) {
        // First char is -
        readNextCharacter = true;
        if (this->currentCharacter == '>') {
            this->appendToToken();
            this->tokenEnd(TOKEN_COMPOSED_PUNCTUATION_ARROW);
        } else {
            this->tokenEnd(TOKEN_PUNCTUATION_MINUS);
            readNextCharacter = false;
        }
    }

    void Lexer::lexerStateSlash(bool &readNextCharacter) {
        // First char is /
        // This could be an inline comment, a block comment or a slash.
        readNextCharacter = true;
        if (this->currentCharacter == '/') {
            this->switchState(TOKENIZER_STATE_INLINE_COMMENT);
            this->appendToToken();
        } else if (this->currentCharacter == '*') {
            this->switchState(TOKENIZER_STATE_BLOCK_COMMENT);
            this->appendToToken();
        } else {
            this->tokenEnd(TOKEN_PUNCTUATION_SLASH);
            readNextCharacter = false;
        }
    }

    void Lexer::lexerStateStringLiteral(bool &readNextCharacter) {
        readNextCharacter = true;
        if (this->currentCharacter == '\\') {
            this->enterState(TOKENIZER_STATE_STRING_LITERAL_ESCAPE);
        } else if (this->currentCharacter == '"') {
            this->tokenEnd(TOKEN_LITERAL_STRING);
        } else {
            this->appendToToken();
        }
    }

    void Lexer::lexerStateStringLiteralEscape(bool &readNextCharacter) {
        // Previous character was a \.

        readNextCharacter = true;
        this->leaveState();
        if (this->currentCharacter == '(') {
            // this->stringHasInterpolation = true;
            // this->tokenStringInterpolationOpenParenthesis += 1;
            this->tokenEnd(TOKEN_LITERAL_STRING);
            this->enterState(TOKENIZER_STATE_STRING_LITERAL);
            this->enterState(TOKENIZER_STATE_STRING_LITERAL_INTERPOLATION);
        } else if (this->currentCharacter == 'n') this->appendToToken('\n');
          else if (this->currentCharacter == '"') this->appendToToken('"');
          else if (this->currentCharacter == '\'') this->appendToToken('\'');
          else if (this->currentCharacter == 't') this->appendToToken('\t');
          else if (this->currentCharacter == 'r') this->appendToToken('\r');
          else if (this->currentCharacter == 'f') this->appendToToken('\f');
          else if (this->currentCharacter == 'b') this->appendToToken('\b');
          else if (this->currentCharacter == '\\') this->appendToToken('\\');
        else {
            // TODO diagnose, this is an error.
            std::stringstream ss;
            ss << "Unkown escaping character: '\\"
               << this->currentCharacter
               << '\'';
            logError(ss.str());
        }
    }

    void Lexer::lexerStateCharacterLiteral(bool &readNextCharacter) {
        readNextCharacter = true;
        if (this->currentCharacter == '\\') {
            this->enterState(TOKENIZER_STATE_CHARACTER_LITERAL_ESCAPE);
        } else if (this->currentCharacter == '\'') {
            this->tokenEnd(TOKEN_LITERAL_CHARACTER);
        } else {
            this->appendToToken();
        }
    }

    void Lexer::lexerStateCharacterLiteralEscape(bool &readNextCharacter) {
        // Previous character was a \.

        readNextCharacter = true;
        this->leaveState();
        if (this->currentCharacter == 'n') this->appendToToken('\n');
        else if (this->currentCharacter == '"') this->appendToToken('"');
        else if (this->currentCharacter == '\'') this->appendToToken('\'');
        else if (this->currentCharacter == 't') this->appendToToken('\t');
        else if (this->currentCharacter == 'r') this->appendToToken('\r');
        else if (this->currentCharacter == 'f') this->appendToToken('\f');
        else if (this->currentCharacter == 'b') this->appendToToken('\b');
        else if (this->currentCharacter == '\\') this->appendToToken('\\');
        else {
            // TODO diagnose, this is an error.
            std::stringstream ss;
            ss << "Unkown escaping character: '\\"
               << this->currentCharacter
               << '\'';
            logError(ss.str());
        }
    }

    void Lexer::lexerStateInlineComment(bool &readNextCharacter) {
        readNextCharacter = true;
        if (this->currentCharacter == '\n') {
            this->tokenEnd(TOKEN_COMMENT);
            readNextCharacter = false;
        } else {
            this->appendToToken();
        }
    }

    void Lexer::lexerStateBlockComment(bool &readNextCharacter) {
        readNextCharacter = true;

        unsigned long int length = this->currentSelection.size();
        if (this->currentSelection[length - 2] == '*' &&
                this->currentSelection[length - 1] == '/') {
            this->tokenEnd(TOKEN_COMMENT);
            readNextCharacter = false;
        } else {
            this->appendToToken();
        }
    }

    void Lexer::parseState(const LexerState& state, bool &readNextCharacter) {
        switch (state.type) {
            case TOKENIZER_STATE_STRING_LITERAL_INTERPOLATION:
            case TOKENIZER_STATE_OPEN_PARENTHESIS:
            case TOKENIZER_STATE_START:
                this->lexerStateStart(readNextCharacter);
                break;
            case TOKENIZER_STATE_SYMBOL:
                this->lexerStateSymbol(readNextCharacter);
                break;
            case TOKENIZER_STATE_IDENTIFIER:
                this->lexerStateIdentifier(readNextCharacter);
                break;
            case TOKENIZER_STATE_NUMBER_LITERAL_ZERO:
                this->lexerStateZero(readNextCharacter);
                break;
            case TOKENIZER_STATE_NUMBER_LITERAL:
                this->lexerStateNumber(readNextCharacter);
                break;
            case TOKENIZER_STATE_NUMBER_LITERAL_EXPONENTIAL:
                this->lexerStateNumberExponential(readNextCharacter);
                break;
            case TOKENIZER_STATE_COMPILER_DIRECTIVE:
                this->lexerStateCompilerDirective(readNextCharacter);
                break;
            case TOKENIZER_STATE_SLASH:
                this->lexerStateSlash(readNextCharacter);
                break;
            case TOKENIZER_STATE_INLINE_COMMENT:
                this->lexerStateInlineComment(readNextCharacter);
                break;
            case TOKENIZER_STATE_BLOCK_COMMENT:
                this->lexerStateBlockComment(readNextCharacter);
                break;
            case TOKENIZER_STATE_DOT:
                this->lexerStateDot(readNextCharacter);
                break;
            case TOKENIZER_STATE_DASH:
                this->lexerStateDash(readNextCharacter);
                break;
            case TOKENIZER_STATE_STRING_LITERAL:
                this->lexerStateStringLiteral(readNextCharacter);
                break;
            case TOKENIZER_STATE_STRING_LITERAL_ESCAPE:
                this->lexerStateStringLiteralEscape(readNextCharacter);
                break;
            case TOKENIZER_STATE_CHARACTER_LITERAL:
                this->lexerStateCharacterLiteral(readNextCharacter);
                break;
            case TOKENIZER_STATE_CHARACTER_LITERAL_ESCAPE:
                this->lexerStateCharacterLiteralEscape(readNextCharacter);
                break;
            default:
                readNextCharacter = true;
                break;
        }
    }

    void Lexer::parse() {
        this->inputFileStream->open(file, std::ifstream::in);

        bool readNextCharacter = true;
        for (this->currentIndex = 0; true; this->currentIndex++) {
            if (readNextCharacter) {
                bool read = this->tryToReadNextCharacter();
                if (!read) break;
            }

            if (this->currentCharacter == '\n') {
                this->currentColumn = 1;
                this->currentLine++;
            } else
                this->currentColumn++;

            const LexerState* state = this->getState();
            if (state == nullptr)
                break;

            this->parseState(*state, readNextCharacter);
        }

        this->appendEndOfFileToken();
        if (this->inputFileStream != nullptr &&
            this->inputFileStream->is_open())
            this->inputFileStream->close();

        safelyDeletePointer(this->inputFileStream);
    }
}
