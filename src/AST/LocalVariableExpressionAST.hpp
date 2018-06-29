//
//  LocalVariableExpressionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef LocalVariableExpressionAST_hpp
#define LocalVariableExpressionAST_hpp

#include "ExpressionAST.hpp"
#include <string>
#include <memory>

namespace warriorlang {
    /**
     Represents a local variable declaration.

     ```warriorlang
     let name: String = "Rafael Guerreiro";
     var age: UInt8 = 26;
     ```
     */
    class LocalVariableExpressionAST: public ExpressionAST {
    private:
        bool isMutable;
        std::string name;
        std::string type;
        std::unique_ptr<ExpressionAST> value;
    public:
        ~LocalVariableExpressionAST();
        LocalVariableExpressionAST(bool isMutable,
                                   const std::string &name,
                                   const std::string &type,
                                   std::unique_ptr<ExpressionAST> &value) {
            this->isMutable = isMutable;
            this->name = name;
            this->type = type;
            this->value = std::move(value);
        }
    };
}
#endif /* LocalVariableExpressionAST_hpp */
