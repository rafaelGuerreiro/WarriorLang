//
//  TokenCategory.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-26.
//

#ifndef TokenCategory_hpp
#define TokenCategory_hpp

namespace warriorlang {
    enum TokenCategory {
        TOKEN_EOF = -1,

        /**
         Class name, Variable name:
            Can be anything, but: var, let, inout, self, super.
         Labels for iterators:
            label: for (var index: Int8 = 0; index < 10; index++) { break label if index % 2 == 0; }
         keywords:
             var,
             let,
             new,            // new Class(argument: "test") -> syntax-sugar to Class.init(argument: "test");
             function,
             public,
             internal,       // internal allows access to the same module
             private,        // private allows access to extensions and inheritance
             publicset,
             internalset,    // internalset allows access to the same module
             privateset,     // privateset allows access to extensions and inheritance
             class,          // class object type or class function
             interface,
             enum,
             annotation,
             module,         // Modules can be used to automatically insert common methods.
             extension,
             operator,
             overridable,    // all classes and functions are final. Use overridable to mark them as overridable.
             override,       // when overriding a super function
             null,           // Same as Optional<T>.none
             self,
             super,
             type,           // Like Swift's Self
             mutating,       // mutating functions allows this function to modify the underlying object
             mutable,        // mutable returns allows the returned instance to be assigned to `var` and be modified.
             required,       // Forces sub-implementation to override this method. Implies it's also overridable. Therefore, it can't call any self mutating methods.
             inout,          // Makes an argument mutable, which also reflects on the caller instance. `let` variables can't be passed to inout.
             include,        // used to include a module in this class and all implementations.
             includes,       // check if this instance or class includes this module.
             where,          // Used to refine generics in functions and extensions.
             throw,
             throws,
             try,
             try?,
             catch,
             finally,        // Is garanteed to be invoked, even when errors happened. This block can't return, but it can throw.
             defer,          // Is garanteed to be invoked, even when errors happened. This block can't return, but it can throw.
             as,
             as?,
             is,             // same as instanceof in Java: return true if variable is String;
             async,
             await,
             if,
             else if,
             else,
             unless,
             guard,
             if let,
             if var,
             guard let,
             guard var,
             while,
             for,
             in,
             do,
             break,
             continue,
             init,           // Init methods can't read from self. Developers should use postconstruct methods.
             init?,          // Init method that can return null.
             deinit,         // When this object is going to be removed from memory, this method is called.
             postconstruct,  // A private method that is called after the complete initialization of the object. Child first.
             return
         */
        TOKEN_IDENTIFIER = -2,

        /**
         Any number literal:
             3.14
             3
             10_000
             10_000.5002030
             0
             -0 is the same as 0
             -1
             -20
             -10_000
             -10_000.5002030
             -3.16
         */
        TOKEN_NUMBER = -3,

        /**
         All known operators:
             +       Addition
             -       Subtraction
             *       Multiplication
             /       Division
             %       Modulus
             ~       Inverse binary
             !       Boolean Not
             .       Dot operator
             &       Binary And
             &&      Boolean And
             |       Binary Or
             ||      Boolean Or
             ^       Exclusive Or (XOR)
             <       Lower Than
             <=      Lower Than Or Equals
             >       Greater Than
             >=      Greater Than Or Equals
             ==      Is equals
             !=      Is different
             ===     Is the same reference
             !==     Is not the same reference
             ?       Optional expansion
             ??      Null Coalescing
             ++      Increase by 1
             --      Decrease by 1
             =       Assignment. Assignment expressions return the new value of the variable.
             +=      var = var + value shortcut
             -=      var = var - value shortcut
             /=      var = var / value shortcut
             *=      var = var * value shortcut
             %=      var = var % value shortcut
             &=      var = var & value shortcut
             |=      var = var | value shortcut
             ~=      var = var ~ value shortcut
             ^=      var = var ^ value shortcut
             ->      Arrow operator (defines return type for functions and closures)
         All user defined operators besides those.

         Open and close curly braces.
         <>

         - When in a class definition, it defines generics for the objects of this type.
         - When in a function definition, it defines generics for this function.
         */
        TOKEN_SYMBOL = -4,

        /**
         Strings are double-quoted.
         - It allows common escaping:
             \"  // " character
             \\  // \ character
             \n  // new line
             \t  // tab
             \$  // $ character that escapes the interpolation
             \b  // backspace
             \r  // carriage return
             \f  // formfeed

         - It allows interpolation:
             "regular string"
             "regular string, value: $singleNamedVariable"
             "regular string, value: ${1 + 2 / 4 * 7}"
         */
        TOKEN_STRING = -5,

        /**
         Characters are single quoted and single value.

         var char: Character = 'a';

         - It allows common escaping:
             \'  // ' character
             \\  // \ character
             \n  // new line
             \t  // tab
             \b  // backspace
             \r  // carriage return
             \f  // formfeed
         */
        TOKEN_CHARACTER = -6,

        /**
         Open and close parenthesis.
         ()

         - It defines the arguments set of a function
         - It defines the inline information of a Closure
         - It can also be used to prioritize operators order
         */
        TOKEN_PARENTHESIS = -7,

        /**
         Open and close curly braces.
         {}

         - It defines a block of code
         - It defines an inline Map
         - It defines types boundaries
         */
        TOKEN_CURLY_BRACES = -8,

        /**
         Colon operator
         :

         - It can design inheritance and implementation of interfaces when in a type declaration
         - It separates the variable name from its type
         - It separates the argument name from its type
         - It separates the label name from the underlying iterator
         */
        TOKEN_COLON = -9,

        /**
         Semicolon
         ;

         - Separate instructions.
         */
        TOKEN_SEMICOLON = -10,

        /**
         Comma
         ,

         - Separate instructions.
         */
        TOKEN_COMMA = -11,

        /**
         Compiler directive
         #

         - Compiler instructions.
         */
        TOKEN_COMPILER_DIRECTIVE = -12,

        /**
         Comment (escaped below)
         //
         /*
         *\/
         */
        TOKEN_COMMENT = -13,
    };
}

#endif /* TokenCategory_hpp */
