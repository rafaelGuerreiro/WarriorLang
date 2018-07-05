//
//  Lexer.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Parser/Lexer.hpp"
#include "warriorlang/utils.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace warriorlang {
    class LexerTest : public ::testing::Test {
    protected:
        LexerTest() {
            // You can do set-up work for each test here.
        }

        virtual ~LexerTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).

            // When done redirect cout to its old self
            // std::string output = testing::internal::GetCapturedStdout();
        }

        // Objects declared here can be used by all tests in the test case for Foo.
    };

    // Tests that the Foo::Bar() method does Abc.
    TEST_F(LexerTest, LexerHello) {
        // const std::string input_filepath = ;
        Lexer *lexer = new Lexer("/Users/rguerreiro/workspace/llvm-workspace/warriorlang/temp/hello.warlang");
        lexer->parse();

        const std::vector<Token> tokens = lexer->getTokens();
        for (unsigned long int index = 0; index < tokens.size(); index++) {
            const Token token = tokens[index];
            std::cout << "TOKEN: " << token.category << " -> '" << token.value << '\'';

            if (token.category == TOKEN_LITERAL_INTEGER || token.category == TOKEN_LITERAL_FLOAT)
                std::cout << "\t\tradix: " << token.numberRadix << " floating point? " << token.numberFloatingPoint;

            if (token.category == TOKEN_LITERAL_STRING)
                std::cout << "\t\tInterpolation: " << token.stringHasInterpolation;

            std::cout << '\n';
        }

        unsigned long int expectedSize = 1;
        EXPECT_EQ(expectedSize, tokens.size());

        safelyDeletePointer(lexer);
    }

    // Tests that Foo does Xyz.
    // TEST_F(LexerTest, DoesXyz) {
    //     // Exercises the Xyz feature of Foo.
    // }
}

int main(int argc, char **argv) {
    signal(SIGSEGV, warriorlang::exceptionHandler);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}