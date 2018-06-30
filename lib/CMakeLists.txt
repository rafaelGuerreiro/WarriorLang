# Do not use file globing because then CMake is not able to tell whether a file
# has been deleted or added when rebuilding the project.
add_library("${PROJECT_NAME}_lib"
                "Frontend/Tokenizer.cpp"
                "AST/StringLiteralAST.cpp"
                "AST/FunctionMetadataAST.cpp"
                "AST/UInt16LiteralAST.cpp"
                "AST/LocalVariableExpressionAST.cpp"
                "AST/BooleanLiteralAST.cpp"
                "AST/Int32LiteralAST.cpp"
                "AST/FunctionAST.cpp"
                "AST/BinaryExpressionAST.cpp"
                "AST/Int16LiteralAST.cpp"
                "AST/UInt32LiteralAST.cpp"
                "AST/Float64LiteralAST.cpp"
                "AST/Int64LiteralAST.cpp"
                "AST/CharacterLiteralAST.cpp"
                "AST/TypeMetadataAST.cpp"
                "AST/Int8LiteralAST.cpp"
                "AST/ClosureLiteralAST.cpp"
                "AST/UnaryExpressionAST.cpp"
                "AST/ClosureMetadataAST.cpp"
                "AST/FieldMetadataAST.cpp"
                "AST/Float32LiteralAST.cpp"
                "AST/FunctionCallExpressionAST.cpp"
                "AST/UInt64LiteralAST.cpp"
                "AST/UInt8LiteralAST.cpp"
                "AST/ArgumentMetadataAST.cpp")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} "${PROJECT_NAME}_lib" ${llvm_libs})