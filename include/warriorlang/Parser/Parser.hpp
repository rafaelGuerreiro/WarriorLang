//
//  Parser.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-07-06.
//

#ifndef Parser_hpp
#define Parser_hpp

#include "TokenCategory.hpp"
#include "Lexer.hpp"

#include <string>
#include <vector>
namespace warriorlang {
    class Parser {
    private:
        // std::vector<NodeAST>* nodes; // Nodes are basically types because everything should be type.
        const std::vector<Token>* tokens;
    public:
        void parse();
    };
}
#endif