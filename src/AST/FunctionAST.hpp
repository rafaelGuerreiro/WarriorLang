//
//  FunctionAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef FunctionAST_hpp
#define FunctionAST_hpp

#include "ExpressionAST.hpp"
#include "FunctionMetadataAST.hpp"
#include <memory>

namespace warriorlang {

    /**
     Represents a function in WarriorLang:

     ```warriorlang
     public static function name(argument1: String, argument2: UInt8) -> String {
        return "argument1: $argument1, argument2: ${argument2 + 10}";
     }
     ```
     */
    class FunctionAST {
    private:
        std::unique_ptr<FunctionMetadataAST> metadata;
        std::unique_ptr<ExpressionAST> body;
    public:
        ~FunctionAST();
        FunctionAST(std::unique_ptr<FunctionMetadataAST> &metadata,
                    std::unique_ptr<ExpressionAST> &body) {
            this->metadata = std::move(metadata);
            this->body = std::move(body);
        }
    };
}
#endif /* FunctionAST_hpp */
