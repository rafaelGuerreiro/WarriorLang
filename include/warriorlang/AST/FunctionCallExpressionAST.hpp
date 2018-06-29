//
//  FunctionCallExpressionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef FunctionCallExpressionAST_hpp
#define FunctionCallExpressionAST_hpp

#include "ExpressionAST.hpp"
#include <string>
#include <memory>
#include <vector>

namespace warriorlang {
    class FunctionCallExpressionAST: public ExpressionAST {
    private:
        std::string callee;
        std::vector<std::unique_ptr<ExpressionAST>> arguments;

    public:
        ~FunctionCallExpressionAST();
        FunctionCallExpressionAST(const std::string &callee,
                                  std::vector<std::unique_ptr<ExpressionAST>> &arguments) {
            this->callee = callee;
            this->arguments = std::move(arguments);
        }
    };
}
#endif /* FunctionCallExpressionAST_hpp */
