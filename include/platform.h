#pragma once

#ifdef _WIN32
    #define GLUA_C_API_PLATFORM_WINDOWS
    #ifdef _WIN64
        #define GLUA_C_API_ARCHITECTURE_X64
    #else
        #define GLUA_C_API_ARCHITECTURE_X86
    #endif
#elif defined(__linux__)
    #define GLUA_C_API_PLATFORM_LINUX
    #ifdef __x86_64__
        #define GLUA_C_API_ARCHITECTURE_X64
    #else
        #define GLUA_C_API_ARCHITECTURE_X86
    #endif
#else
    #error "Unsupported platform"
#endif
