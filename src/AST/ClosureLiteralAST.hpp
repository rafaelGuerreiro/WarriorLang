//
//  ClosureLiteralAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-24.
//

#ifndef ClosureLiteralAST_hpp
#define ClosureLiteralAST_hpp

#include "ExpressionAST.hpp"
#include "ClosureMetadataAST.hpp"
#include <vector>
#include <memory>

namespace warriorlang {
    /**
     Represents Closure literal in WarriorLang:

     ```warriorlang
     let nameAndAge: Closure = (name: String, age: UInt8) -> String {
        return "Name: $name, age: $age";
     }; // Closure literal

     nameAndAge(name: "Rafael", age: 26); //Invocation
     ```
     */
    class ClosureLiteralAST: public ExpressionAST {
    private:
        std::shared_ptr<ClosureMetadataAST> metadata;
        std::unique_ptr<ExpressionAST> body;

    public:
        ~ClosureLiteralAST();
        ClosureLiteralAST(const std::shared_ptr<ClosureMetadataAST> &metadata,
                          std::unique_ptr<ExpressionAST> &body) {
            this->metadata = std::move(metadata);
            this->body = std::move(body);
        }
    };
}

#endif /* ClosureLiteralAST_hpp */
