//
//  AccessControlAST.hpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-06-23.
//

#ifndef AccessControlAST_hpp
#define AccessControlAST_hpp

namespace warriorlang {
    enum ReadAccessControlAST {
        ACCESS_PRIVATE = 1, ACCESS_INTERNAL = 2, ACCESS_PUBLIC = 3
    };

    enum WriteAccessControlAST {
        ACCESS_PRIVATESET = 1, ACCESS_INTERNALSET = 2, ACCESS_PUBLICSET = 3
    };
}

#endif /* AccessControlAST_hpp */
