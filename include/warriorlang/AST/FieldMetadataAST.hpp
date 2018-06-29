//
//  FieldMetadataAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef FieldMetadataAST_hpp
#define FieldMetadataAST_hpp

#include "AccessControlAST.hpp"
#include "ExpressionAST.hpp"
#include <string>
#include <vector>
#include <memory>

namespace warriorlang {
    class TypeMetadataAST;

    /**
     Represents the metadata of a field declaration in WarriorLang:

     ```warriorlang
     public let field: String = "The inline initialization of the field.";
     private var field2: UInt8;
     internal var field3: UInt8;
     public internalset var field2: UInt8;
     ```

     By definition:
        - All `var` fields are `privateset` unless told otherwise;
        - All fields are `private` unless told otherwise;
        - It is illegal to declare a writeAccess larger than readAccess;
        - All `let` fields are required to be `privateset`, they must be set only once;
        - All `let` fields must be initialized;
        - All non-optional `var` fields must be initialized;
     */
    class FieldMetadataAST {
    private:
        ReadAccessControlAST readAccess;
        WriteAccessControlAST writeAccess;
        bool isStaticType;
        bool isMutable;
        std::string name;
        std::shared_ptr<TypeMetadataAST> type;
        std::unique_ptr<ExpressionAST> inlineInitialization;

    public:
        ~FieldMetadataAST();
        FieldMetadataAST(ReadAccessControlAST readAccess,
                         WriteAccessControlAST writeAccess,
                         bool isStaticType,
                         bool isMutable,
                         const std::string &name,
                         const std::shared_ptr<TypeMetadataAST> &type,
                         std::unique_ptr<ExpressionAST> &inlineInitialization) {
            assert(readAccess >= writeAccess);

            this->readAccess = readAccess;
            this->writeAccess = writeAccess;
            this->isStaticType = isStaticType;
            this->isMutable = isMutable;
            this->name = name;
            this->type = std::move(type);
            this->inlineInitialization = std::move(inlineInitialization);
        }
    };
}

#endif /* FieldMetadataAST_hpp */
