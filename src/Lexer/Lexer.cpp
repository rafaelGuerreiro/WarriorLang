//
//  Lexer.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-25.
//

#include "Lexer.hpp"
// #include "llvm/ADT/STLExtras.h"
#include "WarLangToken.hpp"
#include <iostream>
#include <memory>
#include <fstream>

namespace warriorlang {
    Lexer::~Lexer() { }

    const std::unique_ptr<WarLangToken>& Lexer::createToken(const std::string &file, unsigned int &line, TokenCategory category, const std::string &value) {
        std::unique_ptr<WarLangToken> result = nullptr;// llvm::make_unique<WarLangToken>(file, line, category, value);
        return std::move(result);
    }

    const std::unique_ptr<WarLangToken>& Lexer::createPossibleCommentToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        unsigned int firstLine = line;
        bool comment = false;
        bool lineComment = false;

        std::string value = carryOver;

        char currentChar;
        stream >> currentChar;

        if (currentChar == '/' || currentChar == '*') {
            comment = true;
            lineComment = currentChar == '/';
            value += currentChar;
        }

        if (!comment)
            return createSymbolToken(file, stream, carryOver, line);

        bool incrementedLine = false;
        char lastChar = currentChar;
        while(stream >> currentChar) {
            bool lineBreak = (currentChar == '\n' || currentChar == '\r' || currentChar == '\f');
            if (!incrementedLine && lineBreak) {
                line++;
                incrementedLine = true;
            } else if (!lineBreak)
                incrementedLine = false;

            if (lineComment && lineBreak)
                break;

            value += currentChar;
            if (!lineComment && lastChar == '*' && currentChar == '/')
                break;

            lastChar = currentChar;
        }

        return createToken(file, firstLine, TOKEN_COMMENT, value);
    }

    const std::unique_ptr<WarLangToken>& Lexer::createNumberToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        bool hasDot = false;

        std::string value = carryOver;
        char currentChar;

        while (stream >> currentChar) {
            if (currentChar == '_' || isdigit(currentChar))
                value += currentChar;
            else if (!hasDot && currentChar == '.') {
                hasDot = true;
                value += currentChar;
            } else {
                carryOver += std::string(1, currentChar);
                break;
            }
        }

        return createToken(file, line, TOKEN_NUMBER, value);
    }

    const std::unique_ptr<WarLangToken>& Lexer::createIdentifierToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        std::string value = carryOver;
        char currentChar;

        while (stream >> currentChar) {
            if (isspace(currentChar))
                break;

            if (!(isdigit(currentChar) || isalpha(currentChar) || currentChar == '$' || currentChar == '_')) {
                carryOver = std::string(1, currentChar);
                break;
            }

            value += currentChar;
        }

        return createToken(file, line, TOKEN_IDENTIFIER, value);
    }

    const std::unique_ptr<WarLangToken>& Lexer::createCompilerDirectiveToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        return createToken(file, line, TOKEN_COMPILER_DIRECTIVE, "");
    }

    const std::unique_ptr<WarLangToken>& Lexer::createStringToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        return createToken(file, line, TOKEN_STRING, "");
    }

    const std::unique_ptr<WarLangToken>& Lexer::createCharacterToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line) {
        return createToken(file, line, TOKEN_CHARACTER, "");
    }

    const std::unique_ptr<WarLangToken>& Lexer::createSymbolToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line, char currentChar) {
        std::string value = carryOver;

        if (currentChar != 0) {
            if (isspace(currentChar))
                return createToken(file, line, TOKEN_SYMBOL, value);

            if (isdigit(currentChar) ||
                isalpha(currentChar) ||
                currentChar == '$' ||
                currentChar == '#' ||
                currentChar == '_' ||
                currentChar == '\'' ||
                currentChar == '"' ||
                currentChar == '(' ||
                currentChar == ')' ||
                currentChar == '{' ||
                currentChar == '}' ||
                currentChar == ':' ||
                currentChar == ';' ||
                currentChar == ',') {
                carryOver = std::string(1, currentChar);
                return createToken(file, line, TOKEN_SYMBOL, value);
            }

            value += currentChar;
        }

        while (stream >> currentChar) {
            if (isspace(currentChar))
                break;

            if (isdigit(currentChar) ||
                isalpha(currentChar) ||
                currentChar == '$' ||
                currentChar == '#' ||
                currentChar == '_' ||
                currentChar == '\'' ||
                currentChar == '"' ||
                currentChar == '(' ||
                currentChar == ')' ||
                currentChar == '{' ||
                currentChar == '}' ||
                currentChar == ':' ||
                currentChar == ';' ||
                currentChar == ',') {
                carryOver = std::string(1, currentChar);
                break;
            }

            value += currentChar;
        }

        return createToken(file, line, TOKEN_SYMBOL, value);
    }

    void Lexer::processFile(const std::string &file) {
        std::ifstream stream;
        stream.open(file);
        if (!stream) {
            std::cout << "Unable to open file " << file << std::endl;
            return;
        }

        unsigned int line = 0;
        bool incrementedLine = false;
        char currentChar;
        std::string identifier = "";

        while (stream >> currentChar) {
            if (isspace(currentChar)) {
                if (!incrementedLine && (currentChar == '\n' || currentChar == '\r' || currentChar == '\f')) {
                    line++;
                    incrementedLine = true;
                }
                continue;
            }

            incrementedLine = false;
            identifier += currentChar;

            if (currentChar == '/') {
                // This could be a comment or an operator
                this->tokens.push_back(createPossibleCommentToken(file, stream, identifier, line));
            } else if (isdigit(currentChar)) {
                this->tokens.push_back(createNumberToken(file, stream, identifier, line));
            } else if (currentChar == '#') {
                this->tokens.push_back(createSymbolToken(file, stream, identifier, line));
            } else if (isalnum(currentChar) || currentChar == '_' || currentChar == '$') {
                this->tokens.push_back(createSymbolToken(file, stream, identifier, line));
            } else if (currentChar == '"') {
                this->tokens.push_back(createStringToken(file, stream, identifier, line));
            } else if (currentChar == '\'') {
                this->tokens.push_back(createCharacterToken(file, stream, identifier, line));
            } else if (currentChar == '(' || currentChar == ')') {
                this->tokens.push_back(createToken(file, line, TOKEN_PARENTHESIS, identifier));
            } else if (currentChar == '{' || currentChar == '}') {
                this->tokens.push_back(createToken(file, line, TOKEN_CURLY_BRACES, identifier));
            } else if (currentChar == ':') {
                this->tokens.push_back(createToken(file, line, TOKEN_COLON, identifier));
            } else if (currentChar == ';') {
                this->tokens.push_back(createToken(file, line, TOKEN_SEMICOLON, identifier));
            } else if (currentChar == ',') {
                this->tokens.push_back(createToken(file, line, TOKEN_COMMA, identifier));
            } else {
                this->tokens.push_back(createSymbolToken(file, stream, identifier, line));
            }
        }

        stream.close();
    }
}
