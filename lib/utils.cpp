//
//  utils.cpp
//  WarriorLang
//
//  Created by Rafael Guerreiro on 2018-07-01.
//

#include "warriorlang/utils.hpp"

#include <execinfo.h> // for backtrace
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace warriorlang {

    void exceptionHandler(int sig) {
        void *callstack[128];
        const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
        char buf[1024];
        int nFrames = backtrace(callstack, nMaxFrames);
        char **symbols = backtrace_symbols(callstack, nFrames);

        fprintf(stderr, "Error: signal %d:\n", sig);

        std::ostringstream trace_buf;
        for (int i = 1; i < nFrames; i++) {
            printf("%s\n", symbols[i]);

            Dl_info info;
            if (dladdr(callstack[i], &info) && info.dli_sname) {
                char *demangled = NULL;
                int status = -1;
                if (info.dli_sname[0] == '_')
                    demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
                snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd\n",
                        i, int(2 + sizeof(void*) * 2), callstack[i],
                        status == 0 ? demangled :
                        info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                        (char *)callstack[i] - (char *)info.dli_saddr);
                free(demangled);
            } else {
                snprintf(buf, sizeof(buf), "%-3d %*p %s\n",
                        i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
            }
            trace_buf << buf;
        }
        free(symbols);
        if (nFrames == nMaxFrames)
            trace_buf << "[truncated]\n";

        fprintf(stderr, "%s", std::string(trace_buf.str()).c_str());
        exit(1);
    }

    void safelyDeletePointer(void *pointer) {
        delete pointer;
        pointer = nullptr;
    }
}