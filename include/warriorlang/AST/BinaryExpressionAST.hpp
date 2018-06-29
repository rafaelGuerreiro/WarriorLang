//
//  BinaryExpressionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef BinaryExpressionAST_hpp
#define BinaryExpressionAST_hpp

#include "ExpressionAST.hpp"
#include <string>
#include <memory>

namespace warriorlang {
    /**
     Represents binary expressions in WarriorLang:

     ```warriorlang
     let left: Int32 = 5;
     let right: Int32 = 10;
     let result: Int32 = left + right; // Binary operation
     print(result);
     ```
     */
    class BinaryExpressionAST: public ExpressionAST {
    private:
        std::string operatorName;
        std::unique_ptr<ExpressionAST> left;
        std::unique_ptr<ExpressionAST> right;
    public:
        ~BinaryExpressionAST();
        BinaryExpressionAST(const std::string &operatorName,
                            std::unique_ptr<ExpressionAST> left,
                            std::unique_ptr<ExpressionAST> right) {
            this->operatorName = operatorName;
            this->left = std::move(left);
            this->right = std::move(right);
        }
    };
}
#endif /* BinaryExpressionAST_hpp */
