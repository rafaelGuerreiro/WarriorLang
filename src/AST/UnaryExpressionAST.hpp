//
//  UnaryExpressionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef UnaryExpressionAST_hpp
#define UnaryExpressionAST_hpp

#include "ExpressionAST.hpp"
#include <string>
#include <memory>

namespace warriorlang {
    class UnaryExpressionAST: public ExpressionAST {
    private:
        std::string operatorName;
        std::unique_ptr<ExpressionAST> target;
    public:
        ~UnaryExpressionAST();
        UnaryExpressionAST(const std::string &operatorName,
                           std::unique_ptr<ExpressionAST> target) {
            this->operatorName = operatorName;
            this->target = std::move(target);
        }
    };
}
#endif /* UnaryExpressionAST_hpp */
