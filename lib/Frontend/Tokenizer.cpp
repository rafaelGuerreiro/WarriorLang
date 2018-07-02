//
//  Tokenizer.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Frontend/Tokenizer.hpp"
#include "warriorlang/utils.hpp"
#include <iostream>

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
        this->currentLine = 0;
        this->currentColumn = 0;

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

    const std::vector<Token> Tokenizer::getTokens() {
        return *this->tokens;
    }

    void Tokenizer::tokenStart() {
        this->tokenStartIndex = this->currentIndex;
        this->tokenEndIndex = this->currentIndex + 1;
        this->tokenStartLine = this->currentLine;
        this->tokenStartColumn = this->currentColumn;
        this->currentSelection = std::string(1, this->currentCharacter);
    }

    void Tokenizer::appendToToken() {
        this->tokenEndIndex = this->currentIndex + 1;
        this->currentSelection += this->currentCharacter;
    }

    void Tokenizer::tokenEnd(const TokenCategory &category) {
        this->state = TOKENIZER_STATE_START;
        this->tokens->push_back(Token {
            /* category: */     category,
            /* value: */        this->currentSelection,
            /* file: */         this->file,
            /* startIndex: */   this->tokenStartIndex,
            /* endIndex: */     this->tokenEndIndex,
            /* startLine: */    this->tokenStartLine,
            /* startColumn: */  this->tokenStartColumn
        });
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
            /* startColumn: */  this->currentColumn
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
            /* startColumn: */  this->currentColumn
        });
    }

    const Token* Tokenizer::peekToken() {
        if (this->tokens->empty())
            return nullptr;

        return &((*this->tokens)[this->tokens->size() - 1]);
    }

    void Tokenizer::tokenize() {
        this->inputFileStream->open(file, std::ifstream::in);

        for (; this->tryToReadNextCharacter(); this->currentIndex++) {
            // std::cout << "FOUND CHAR: " << currentCharacter << '\n';
            switch (this->state) {
                case TOKENIZER_STATE_START:
                    if (isspace(this->currentCharacter)) {
                        this->appendSpaceTokenIfLastTokenWasNotSpaceAlready();
                    } else if (isdigit(this->currentCharacter)) {
                        this->tokenStart();
                    }
                    break;
                default:
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
