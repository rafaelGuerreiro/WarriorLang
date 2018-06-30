//
//  Lexer.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-25.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include "WarLangToken.hpp"
#include "TokenCategory.hpp"
#include <string>
#include <vector>
#include <memory>

namespace warriorlang {
    class Lexer {
    private:
        // std::vector<std::unique_ptr<WarLangToken>> tokens;

        const std::unique_ptr<WarLangToken>& createToken(const std::string &file, unsigned int &line, TokenCategory category, const std::string &value);
        const std::unique_ptr<WarLangToken>& createPossibleCommentToken(const std::string &file, std::ifstream &stream, std::string &number, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createNumberToken(const std::string &file, std::ifstream &stream, std::string &number, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createIdentifierToken(const std::string &file, std::ifstream &stream, std::string &value, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createCompilerDirectiveToken(const std::string &file, std::ifstream &stream, std::string &value, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createStringToken(const std::string &file, std::ifstream &stream, std::string &value, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createCharacterToken(const std::string &file, std::ifstream &stream, std::string &value, unsigned int &line);
        const std::unique_ptr<WarLangToken>& createSymbolToken(const std::string &file, std::ifstream &stream, std::string &carryOver, unsigned int &line, char currentChar = 0);
    public:
        ~Lexer();
        // Lexer(std::vector<std::unique_ptr<WarLangToken>> &tokens) {
            // this->tokens = std::move(tokens);

//            operators.push_back(".");
//            operators.push_back("...");
//            operators.push_back("..<");
//            operators.push_back("->");
//            operators.push_back("===");
//            operators.push_back("!==");
//            operators.push_back("+");
//            operators.push_back("-");
//            operators.push_back("*");
//            operators.push_back("/");
//            operators.push_back("%");
//            operators.push_back("~");
//            operators.push_back("!");
//            operators.push_back("&");
//            operators.push_back("&&");
//            operators.push_back("|");
//            operators.push_back("||");
//            operators.push_back("^");
//            operators.push_back("<");
//            operators.push_back("<=");
//            operators.push_back(">");
//            operators.push_back(">=");
//            operators.push_back("==");
//            operators.push_back("!=");
//            operators.push_back("?");
//            operators.push_back("??");
//            operators.push_back("++");
//            operators.push_back("--");
//            operators.push_back("=");
//            operators.push_back("+=");
//            operators.push_back("-=");
//            operators.push_back("/=");
//            operators.push_back("*=");
//            operators.push_back("%=");
//            operators.push_back("&=");
//            operators.push_back("|=");
//            operators.push_back("~=");
//            operators.push_back("^=");
        // }

        void processFile(const std::string &file);
    };
}
#endif /* Lexer_hpp */
