//
//  FunctionMetadataAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef FunctionMetadataAST_hpp
#define FunctionMetadataAST_hpp

#include "AccessControlAST.hpp"
#include "ArgumentMetadataAST.hpp"
#include <string>
#include <vector>
#include <memory>

namespace warriorlang {
    class TypeMetadataAST;
    /**
     Represents the metadata of a function declaration in WarriorLang:

     ```warriorlang
     public static mutating function name(argument1: Type, argument2: Type) -> ReturnType
     ```
     */
    class FunctionMetadataAST {
    private:
        ReadAccessControlAST access;
        bool isStaticType;
        bool isMutating;
        std::string name;
        std::vector<std::unique_ptr<ArgumentMetadataAST>> arguments;
        std::shared_ptr<TypeMetadataAST> returnType;

    public:
        ~FunctionMetadataAST();
        FunctionMetadataAST(ReadAccessControlAST &access,
                            bool isStaticType,
                            bool isMutating,
                            std::string name,
                            std::vector<std::unique_ptr<ArgumentMetadataAST>> &arguments,
                            const std::shared_ptr<TypeMetadataAST> &returnType) {
            this->access = access;
            this->isStaticType = isStaticType;
            this->isMutating = isMutating;
            this->name = name;
            this->arguments = std::move(arguments);
            this->returnType = std::move(returnType);
        }
    };
}
#endif /* FunctionMetadataAST_hpp */
