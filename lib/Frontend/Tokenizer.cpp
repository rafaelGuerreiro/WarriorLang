//
//  Tokenizer.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Frontend/Tokenizer.hpp"
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
        { "var",            TOKEN_DECLARATION_VAR },
        { "let",            TOKEN_DECLARATION_LET },
        { "get",            TOKEN_DECLARATION_GET },
        { "set",            TOKEN_DECLARATION_SET },
        { "function",       TOKEN_DECLARATION_FUNCTION },
        { "init",           TOKEN_DECLARATION_INIT },
        // { "init?",          TOKEN_DECLARATION_INIT_OPTIONAL },
        { "deinit",         TOKEN_DECLARATION_DEINIT },
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
        { "mutable",        TOKEN_MODIFIER_MUTABLE },
        { "inout",          TOKEN_MODIFIER_INOUT },
        { "where",          TOKEN_MODIFIER_WHERE },
        { "throws",         TOKEN_MODIFIER_THROWS },
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
        { "#if",            TOKEN_COMPILE_DIRECTIVE_IF },
        { "#else",          TOKEN_COMPILE_DIRECTIVE_ELSE },
        { "#elseif",        TOKEN_COMPILE_DIRECTIVE_ELSEIF },
        { "#endif",         TOKEN_COMPILE_DIRECTIVE_ENDIF },
        { "#line",          TOKEN_COMPILE_DIRECTIVE_LINE },
        { "#function",      TOKEN_COMPILE_DIRECTIVE_FUNCTION },
        { "#error",         TOKEN_COMPILE_DIRECTIVE_ERROR },
        { "#warning",       TOKEN_COMPILE_DIRECTIVE_WARNING },
        { "_",              TOKEN_UNDERSCORE }
    };

    static const TokenCategory* getKeywordCategory(const std::string &word) {
        for (unsigned long int index = 0; index < keywords.size(); index++)
            if (word == keywords[index].keyword)
                return &keywords[index].category;

        return nullptr;
    }

    Tokenizer::Tokenizer(const std::string &file) {
        this->file = file;
        this->state = TOKENIZER_STATE_START;
        this->currentCharacter = '\0';
        this->currentIndex = 0;
        this->currentLine = 1;
        this->currentColumn = 0;

        this->numericTokenMetadata = NumericTokenMetadata {
            /* radix: */            0,
            /* floatingPoint: */    false
        };

        this->tokens = new std::vector<Token>();
        this->inputFileStream = new std::ifstream();

        this->tokenStartIndex = 0;
        this->tokenEndIndex = 0;
        this->tokenStartLine = 0;
        this->tokenStartColumn = 0;
        this->currentSelection = "";
    }

    Tokenizer::~Tokenizer() {
        if (this->tokens != nullptr) {
            this->tokens->clear();
            safelyDeletePointer(this->tokens);
        }

        if (this->inputFileStream != nullptr && this->inputFileStream->is_open()) {
            this->inputFileStream->close();
            safelyDeletePointer(this->inputFileStream);
        }
    }

    void Tokenizer::logError(const std::string &message) {
        std::cout << this->file
                  << ':'
                  << this->currentLine
                  << ':'
                  << this->currentColumn
                  << ": error: "
                  << message
                  << '\n';
    }

    const std::vector<Token> Tokenizer::getTokens() {
        return *this->tokens;
    }

    void Tokenizer::tokenStart() {
        this->tokenStartWithoutAddingCurrentCharacter();
        this->currentSelection = std::string(1, this->currentCharacter);
    }

    void Tokenizer::tokenStart(const TokenizerState &state) {
        this->tokenStart();
        this->state = state;
    }

    void Tokenizer::tokenStartWithoutAddingCurrentCharacter() {
        this->tokenStartIndex = this->currentIndex;
        this->tokenEndIndex = this->currentIndex + 1;
        this->tokenStartLine = this->currentLine;
        this->tokenStartColumn = this->currentColumn;
        this->currentSelection = "";
    }

    void Tokenizer::tokenStartWithoutAddingCurrentCharacter(const TokenizerState &state) {
        this->tokenStartWithoutAddingCurrentCharacter();
        this->state = state;
    }

    void Tokenizer::appendToToken(const char &c) {
        this->tokenEndIndex = this->currentIndex + 1;
        this->currentSelection += c;
    }

    void Tokenizer::appendToToken() {
        this->appendToToken(this->currentCharacter);
    }

    void Tokenizer::tokenEnd(const TokenCategory &category) {
        this->state = TOKENIZER_STATE_START;

        int radix = this->numericTokenMetadata.radix;
        bool floatingPoint = this->numericTokenMetadata.floatingPoint;

        if (category != TOKEN_LITERAL_FLOAT && category != TOKEN_LITERAL_INTEGER) {
            radix = 0;
            floatingPoint = false;
        }

        this->tokens->push_back(Token {
            /* category: */     category,
            /* value: */        this->currentSelection,
            /* file: */         this->file,
            /* startIndex: */   this->tokenStartIndex,
            /* endIndex: */     this->tokenEndIndex,
            /* startLine: */    this->tokenStartLine,
            /* startColumn: */  this->tokenStartColumn,
            {
                /* radix: */        radix,
                /* floatingPoint: */floatingPoint
            }
        });
    }

    void Tokenizer::appendSingleCharacterToken(const TokenCategory &category) {
        this->tokenStart();
        this->tokenEnd(category);
    }

    void Tokenizer::appendEndOfFileToken() {
        const Token* lastToken = peekToken();
        if (lastToken != nullptr &&
            lastToken->category == TOKEN_EOF) // Nothing can be added after an EOF.
            return;

        this->tokens->push_back(Token {
            /* category: */     TOKEN_EOF,
            /* value: */        "",
            /* file: */         this->file,
            /* startIndex: */   this->currentIndex,
            /* endIndex: */     this->currentIndex + 1,
            /* startLine: */    this->currentLine,
            /* startColumn: */  this->currentColumn,
            {
                /* radix: */        0,
                /* floatingPoint: */false
            }
        });
    }

    bool Tokenizer::tryToReadNextCharacter() {
        this->currentCharacter = this->inputFileStream->get();
        return this->inputFileStream->good();
    }

    void Tokenizer::appendSpaceTokenIfLastTokenWasNotSpaceAlready() {
        const Token* lastToken = peekToken();
        if (lastToken == nullptr ||
            lastToken->category == TOKEN_EOF || // Nothing can be added after an EOF.
            lastToken->category == TOKEN_SPACE)
            return;

        this->tokens->push_back(Token {
            /* category: */     TOKEN_SPACE,
            /* value: */        " ",
            /* file: */         this->file,
            /* startIndex: */   this->currentIndex,
            /* endIndex: */     this->currentIndex + 1,
            /* startLine: */    this->currentLine,
            /* startColumn: */  this->currentColumn,
            {
                /* radix: */        0,
                /* floatingPoint: */false
            }
        });
    }

    const Token* Tokenizer::peekToken() {
        if (this->tokens->empty())
            return nullptr;

        return &((*this->tokens)[this->tokens->size() - 1]);
    }

    void Tokenizer::tokenizerStateStart(bool &readNextCharacter) {
        readNextCharacter = true;
        if (isspace(this->currentCharacter))
            return;
        else if (this->currentCharacter == '0')
            this->tokenStart(TOKENIZER_STATE_NUMBER_LITERAL_ZERO);
        else if (isdigit(this->currentCharacter))
            this->tokenStart(TOKENIZER_STATE_NUMBER_LITERAL);
        else if (isalpha(this->currentCharacter) ||
                this->currentCharacter == '_' ||
                this->currentCharacter == '$')
            this->tokenStart(TOKENIZER_STATE_SYMBOL);
        else if (this->currentCharacter == '"')
            this->tokenStartWithoutAddingCurrentCharacter(TOKENIZER_STATE_STRING_LITERAL);
        // else if (this->currentCharacter == '\'')
        //     this->tokenStart(TOKENIZER_STATE_CHARACTER_LITERAL);
        else if (this->currentCharacter == '/')
            this->tokenStart(TOKENIZER_STATE_SLASH);
        else if (this->currentCharacter == '-')
            this->tokenStart(TOKENIZER_STATE_DASH);
        // else if (this->currentCharacter == '#')
        //     this->tokenStart(TOKENIZER_STATE_COMPILER_DIRECTIVE); // Could be a compiler directive

        // else if (this->currentCharacter == '>')
        //     this->appendSingleCharacterToken(TOKEN_PUNCTUATION_ARROW, // ->

        // Single char tokens
        else if (this->currentCharacter == '{')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_CURLY_BRACE);
        else if (this->currentCharacter == '}')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_CURLY_BRACE);
        else if (this->currentCharacter == '(')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_PARENTHESIS);
        else if (this->currentCharacter == ')')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_PARENTHESIS);
        else if (this->currentCharacter == '[')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_SQUARE_BRACKETS);
        else if (this->currentCharacter == ']')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_SQUARE_BRACKETS);
        else if (this->currentCharacter == '<')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_LEFT_ANGLE_BRACKETS);
        else if (this->currentCharacter == '>')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_RIGHT_ANGLE_BRACKETS);
        else if (this->currentCharacter == '.')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_DOT);
        else if (this->currentCharacter == ',')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_COMMA);
        else if (this->currentCharacter == ':')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_COLON);
        else if (this->currentCharacter == ';')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_SEMICOLON);
        else if (this->currentCharacter == '+')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_PLUS);
        else if (this->currentCharacter == '-')                         // This could also be the arrow operator
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_MINUS);
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
        else if (this->currentCharacter == '`')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_BACKTICK);
        else if (this->currentCharacter == '=')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_EQUAL);
        else if (this->currentCharacter == '@')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_AT);
        else if (this->currentCharacter == '#')                         // This could also be a compiler directive
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_POUND);
        else if (this->currentCharacter == '&')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_AMPERSAND);
        else if (this->currentCharacter == '\\')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_BACKSLASH);
        else if (this->currentCharacter == '!')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_EXCLAMATION);
        else if (this->currentCharacter == '?')
            this->appendSingleCharacterToken(TOKEN_PUNCTUATION_QUESTION);
    }

    void Tokenizer::tokenizerStateSymbol(bool &readNextCharacter) {
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

    void Tokenizer::tokenizerStateNumber(bool &readNextCharacter) {
        // First char is a digit.
        if (this->numericTokenMetadata.radix == 0)
            this->numericTokenMetadata.radix = 10;

        readNextCharacter = true;

        if (this->currentCharacter == '_')
            this->appendToToken();
        else if (!this->numericTokenMetadata.floatingPoint &&
               this->currentCharacter == '.') {
            this->numericTokenMetadata.floatingPoint = true;
            this->appendToToken();
        }
        else if (this->numericTokenMetadata.radix == 2 &&
                this->currentCharacter >= '0' &&
                this->currentCharacter <= '1')
            this->appendToToken();
        else if (this->numericTokenMetadata.radix == 8 &&
                this->currentCharacter >= '0' &&
                this->currentCharacter <= '7')
            this->appendToToken();
        else if (this->numericTokenMetadata.radix == 10 &&
                this->currentCharacter >= '0' &&
                this->currentCharacter <= '9')
            this->appendToToken();
        else if (this->numericTokenMetadata.radix == 16 &&
                (
                    (this->currentCharacter >= '0' && this->currentCharacter <= '9') ||
                    (this->currentCharacter >= 'a' && this->currentCharacter <= 'f') ||
                    (this->currentCharacter >= 'A' && this->currentCharacter <= 'F')
                ))
            this->appendToToken();
        else {
            TokenCategory category = this->numericTokenMetadata.floatingPoint ?
                                        TOKEN_LITERAL_FLOAT :
                                        TOKEN_LITERAL_INTEGER;

            this->tokenEnd(category);
            readNextCharacter = false;

            this->numericTokenMetadata.radix = 0;
            this->numericTokenMetadata.floatingPoint = false;
        }
    }

    void Tokenizer::tokenizerStateZero(bool &readNextCharacter) {
        // First char is 0.
        readNextCharacter = true;
        this->state = TOKENIZER_STATE_NUMBER_LITERAL;

        if(this->currentCharacter == 'x') {
            this->numericTokenMetadata.radix = 16;
            this->appendToToken();
        } else if (this->currentCharacter == 'o') {
            this->numericTokenMetadata.radix = 8;
            this->appendToToken();
        } else if (this->currentCharacter == 'b') {
            this->numericTokenMetadata.radix = 2;
            this->appendToToken();
        } else if (!this->numericTokenMetadata.floatingPoint &&
                this->currentCharacter == '.') {
            this->numericTokenMetadata.floatingPoint = true;
            this->appendToToken();
        } else if (this->currentCharacter == '_' ||
                isdigit(this->currentCharacter)) {
            this->appendToToken();
        } else {
            this->numericTokenMetadata.radix = 10;
            this->numericTokenMetadata.floatingPoint = false;
            this->tokenEnd(TOKEN_LITERAL_INTEGER);
            readNextCharacter = false;

            this->numericTokenMetadata.radix = 0;
            this->numericTokenMetadata.floatingPoint = false;
        }
    }

    void Tokenizer::tokenizerStateCompilerDirective(bool &readNextCharacter) {
        // First char is #
    }

    void Tokenizer::tokenizerStateDash(bool &readNextCharacter) {
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

    void Tokenizer::tokenizerStateSlash(bool &readNextCharacter) {
        // First char is /
        // This could be an inline comment, a block comment or a slash.
        readNextCharacter = true;
        if (this->currentCharacter == '/') {
            this->state = TOKENIZER_STATE_INLINE_COMMENT;
            this->appendToToken();
        } else if (this->currentCharacter == '*') {
            this->state = TOKENIZER_STATE_BLOCK_COMMENT;
            this->appendToToken();
        } else {
            this->tokenEnd(TOKEN_PUNCTUATION_SLASH);
            readNextCharacter = false;
        }
    }

    void Tokenizer::tokenizerStateStringLiteral(bool &readNextCharacter) {
        readNextCharacter = true;
        if (this->currentCharacter == '\\') {
            this->state = TOKENIZER_STATE_STRING_LITERAL_ESCAPE;
        } else if (this->currentCharacter == '"') {
            this->tokenEnd(TOKEN_LITERAL_STRING);
        } else {
            this->appendToToken();
        }
    }

    void Tokenizer::tokenizerStateStringLiteralEscape(bool &readNextCharacter) {
        // Previous character was a \.

        readNextCharacter = true;
        this->state = TOKENIZER_STATE_STRING_LITERAL;
        if (this->currentCharacter == 'n')
            this->appendToToken('\n');
        else if (this->currentCharacter == '"')
            this->appendToToken('"');
        else if (this->currentCharacter == '\'')
            this->appendToToken('\'');
        else if (this->currentCharacter == 't')
            this->appendToToken('\t');
        else if (this->currentCharacter == 'r')
            this->appendToToken('\r');
        else if (this->currentCharacter == 'f')
            this->appendToToken('\f');
        else if (this->currentCharacter == '\\')
            this->appendToToken('\\');
        else if (this->currentCharacter == '\b')
            this->appendToToken('\b');
        else {
            // TODO diagnose, this is an error.
            std::stringstream ss;
            ss << "Unkown escaping character: '\\"
               << this->currentCharacter
               << '\'';
            logError(ss.str());
        }
    }

    void Tokenizer::tokenizerStateCharacterLiteral(bool &readNextCharacter) {

    }

    void Tokenizer::tokenizerStateInlineComment(bool &readNextCharacter) {
        readNextCharacter = true;
        if (this->currentCharacter == '\n') {
            this->tokenEnd(TOKEN_COMMENT);
            readNextCharacter = false;
        } else {
            this->appendToToken();
        }
    }

    void Tokenizer::tokenizerStateBlockComment(bool &readNextCharacter) {
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

    void Tokenizer::tokenize() {
        this->inputFileStream->open(file, std::ifstream::in);

        bool readNextCharacter = true;
        for (this->currentIndex = 0; true; this->currentIndex++) {
            // std::cout << "FOUND CHAR: " << currentCharacter << '\n';

            if (readNextCharacter) {
                bool read = this->tryToReadNextCharacter();
                if (!read) break;
            }

            if (this->currentCharacter == '\n') {
                this->currentColumn = 0;
                this->currentLine++;
            } else
                this->currentColumn++;

            switch (this->state) {
                case TOKENIZER_STATE_START:
                    this->tokenizerStateStart(readNextCharacter);
                    break;
                case TOKENIZER_STATE_SYMBOL:
                    this->tokenizerStateSymbol(readNextCharacter);
                    break;
                case TOKENIZER_STATE_NUMBER_LITERAL_ZERO:
                    this->tokenizerStateZero(readNextCharacter);
                    break;
                case TOKENIZER_STATE_NUMBER_LITERAL:
                    this->tokenizerStateNumber(readNextCharacter);
                    break;
                // case TOKENIZER_STATE_COMPILER_DIRECTIVE:
                //     this->tokenizerStateCompilerDirective(readNextCharacter);
                //     break;
                case TOKENIZER_STATE_SLASH:
                    this->tokenizerStateSlash(readNextCharacter);
                    break;
                case TOKENIZER_STATE_INLINE_COMMENT:
                    this->tokenizerStateInlineComment(readNextCharacter);
                    break;
                case TOKENIZER_STATE_BLOCK_COMMENT:
                    this->tokenizerStateBlockComment(readNextCharacter);
                    break;
                case TOKENIZER_STATE_DASH:
                    this->tokenizerStateDash(readNextCharacter);
                    break;
                case TOKENIZER_STATE_STRING_LITERAL:
                    this->tokenizerStateStringLiteral(readNextCharacter);
                    break;
                case TOKENIZER_STATE_STRING_LITERAL_ESCAPE:
                    this->tokenizerStateStringLiteralEscape(readNextCharacter);
                    break;
                // case TOKENIZER_STATE_CHARACTER_LITERAL:
                //     this->tokenizerStateCharacterLiteral(readNextCharacter);
                //     break;
                default:
                    readNextCharacter = true;
                    break;
            }
        }

        this->appendEndOfFileToken();
        if (this->inputFileStream != nullptr &&
            this->inputFileStream->is_open())
            this->inputFileStream->close();

        safelyDeletePointer(this->inputFileStream);
    }
}
