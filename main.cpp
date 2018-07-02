//
//  main.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Frontend/Tokenizer.hpp"
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
    Tokenizer* tokenizer = new Tokenizer("/Users/rguerreiro/workspace/llvm-workspace/warriorlang/temp/hello.warlang");
    tokenizer->tokenize();

    delete tokenizer;
    return 0;
}