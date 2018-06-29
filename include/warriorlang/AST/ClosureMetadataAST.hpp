//
//  ClosureMetadataAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef ClosureMetadataAST_hpp
#define ClosureMetadataAST_hpp

#include "ArgumentMetadataAST.hpp"
#include <string>
#include <vector>
#include <memory>

namespace warriorlang {
    /**
     Represents the metadata of a Closure declaration in WarriorLang:

     ```warriorlang
     let nameAndAge: Closure = (name: String, age: UInt8) -> String // Closure declaration
     ```
     */
    class ClosureMetadataAST {
    private:
        std::vector<std::unique_ptr<ArgumentMetadataAST>> arguments;
        std::shared_ptr<TypeMetadataAST> returnType;

    public:
        ~ClosureMetadataAST();
        ClosureMetadataAST(std::vector<std::unique_ptr<ArgumentMetadataAST>> &arguments,
                           const std::shared_ptr<TypeMetadataAST> &returnType) {
            this->arguments = std::move(arguments);
            this->returnType = std::move(returnType);
        }
    };
}

#endif /* ClosureMetadataAST_hpp */
