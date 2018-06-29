//
//  ArgumentsMetadataAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef ArgumentMetadataAST_hpp
#define ArgumentMetadataAST_hpp

#include "ExpressionAST.hpp"
#include <string>
#include <memory>

namespace warriorlang {
    class TypeMetadataAST;
    /**
     Represents a function declaration in WarriorLang:

     ```warriorlang
     public static function name(argument1: Float64, argument2: String = "Default value") -> UInt64
     ```
     */
    class ArgumentMetadataAST {
    private:
        std::string name;
        std::shared_ptr<TypeMetadataAST> type;
        std::unique_ptr<ExpressionAST> defaultValue;

    public:
        ~ArgumentMetadataAST();
        ArgumentMetadataAST(const std::string &name,
                            const std::shared_ptr<TypeMetadataAST> &type,
                            std::unique_ptr<ExpressionAST> &defaultValue) {
            this->name = name;
            this->type = std::move(type);
            this->defaultValue = std::move(defaultValue);
        }
    };
}
#endif /* ArgumentMetadataAST_hpp */
