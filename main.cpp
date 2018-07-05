//
//  main.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Parser/Lexer.hpp"
#include "warriorlang/utils.hpp"
#include <iostream>

using namespace warriorlang;

int main(int length, char** args) {
    std::cout << "warriorlang v0.0.1\n";

    // std::cout << "number of arguments: " << length << "\n";
    // if (length != 1) {
    //     std::cout << "A warlang file must be provided as the first argument.\n" << std::flush;
    //     return 1;
    // }

    // std::string file = std::string(args[0]);
    Lexer* lexer = new Lexer("/Users/rguerreiro/workspace/llvm-workspace/warriorlang/temp/hello.warlang");
    lexer->parse();

    safelyDeletePointer(lexer);
    return 0;
}