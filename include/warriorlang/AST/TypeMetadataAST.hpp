//
//  TypeMetadataAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef TypeMetadataAST_hpp
#define TypeMetadataAST_hpp

#include "AccessControlAST.hpp"
#include "FunctionMetadataAST.hpp"
#include "FieldMetadataAST.hpp"
#include <string>
#include <memory>
#include <vector>

namespace warriorlang {
    enum TypeMetadataType {
        CLASS = 1, INTERFACE, ENUM, ANNOTATION //, OPERATOR, STRUCT
    };

    /**
     Represents a type declaration in WarriorLang:

     ```warriorlang
     class AClass {}
     interface AnInterface {}
     enum AnEnum {}
     annotation AnAnnotation {}

     Not exactly type:
     operator AnOperator {}
     struct AStruct {}
     ```
     */
    class TypeMetadataAST {
    private:
        ReadAccessControlAST access;
        TypeMetadataType type;
        std::string name;
        std::vector<std::shared_ptr<TypeMetadataAST>> interfaces = std::vector<std::shared_ptr<TypeMetadataAST>>();
        std::vector<std::unique_ptr<FieldMetadataAST>> fields = std::vector<std::unique_ptr<FieldMetadataAST>>();
        std::vector<std::unique_ptr<FunctionMetadataAST>> functions = std::vector<std::unique_ptr<FunctionMetadataAST>>();

    public:
        ~TypeMetadataAST();
        TypeMetadataAST(const ReadAccessControlAST &access,
                        const TypeMetadataType &type,
                        const std::string &name) {
            this->access = access;
            this->type = type;
            this->name = name;
        }
    };
}
#endif /* TypeMetadataAST_hpp */
