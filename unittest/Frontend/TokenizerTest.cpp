//
//  Tokenizer.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-29.
//

#include "warriorlang/Frontend/Tokenizer.hpp"
#include "warriorlang/utils.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace warriorlang {
    class TokenizerTest : public ::testing::Test {
    protected:
        TokenizerTest() {
            // You can do set-up work for each test here.
        }

        virtual ~TokenizerTest() {
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
    TEST_F(TokenizerTest, TokenizeHello) {
        // const std::string input_filepath = ;
        Tokenizer *tokenizer = new Tokenizer("/Users/rguerreiro/workspace/llvm-workspace/warriorlang/temp/hello.warlang");
        tokenizer->tokenize();

        const std::vector<Token> tokens = tokenizer->getTokens();
        for (unsigned long int index = 0; index < tokens.size(); index++) {
            const Token token = tokens[index];
            std::cout << "TOKEN: " << token.category << " -> '" << token.value;

            if (token.category == TOKEN_LITERAL_INTEGER || token.category == TOKEN_LITERAL_FLOAT)
                std::cout << "\t\tradix: " << token.numericTokenMetadata.radix << " floating point? '" << token.numericTokenMetadata.floatingPoint;

            std::cout << "'\n";
        }

        unsigned long int expectedSize = 1;
        EXPECT_EQ(expectedSize, tokens.size());

        safelyDeletePointer(tokenizer);
    }

    // Tests that Foo does Xyz.
    // TEST_F(TokenizerTest, DoesXyz) {
    //     // Exercises the Xyz feature of Foo.
    // }
}

int main(int argc, char **argv) {
    signal(SIGSEGV, warriorlang::exceptionHandler);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}