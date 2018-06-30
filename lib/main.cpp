//
//  main.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "Frontend/Tokenizer.cpp"

#include <iostream>

using namespace warriorlang;

int main(int length, char** args) {
    std::cout << "warriorlang v0.0.1" << std::endl;

    if (length != 1) {
        std::cout << "A warlang file must be provided as the first argument." << std::endl;
        return 1;
    }

    return 0;
}