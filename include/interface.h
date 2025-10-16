#pragma once

#include "platform.h"
#include "lua_base.h"

#if defined(GLUA_C_API_ARCHITECTURE_X86)
#define LUA_STATE_COMMON_HEADER_SIZE 70
#elif defined(GLUA_C_API_ARCHITECTURE_X64)
#define LUA_STATE_COMMON_HEADER_SIZE 114
#endif

struct lua_State
{
    unsigned char _common_lua_header_[LUA_STATE_COMMON_HEADER_SIZE];
    
    lua_Base* luaBase;
}

#if defined(GLUA_C_API_PLATFORM_WINDOWS)
#define GMOD_DLL_EXPORT extern "C" __declspec(dllexport)
#elif defined(GLUA_C_API_PLATFORM_LINUX)
#define GMOD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#define GMOD_MODULE_OPEN()                     \
int __gmod13_open(lua_Base* LUA);              \
GMOD_DLL_EXPORT int gmod13_open(lua_State* L)  \
{                                              \
    return __gmod13_open(L->luaBase);          \
}                                              \
int __gmod13_open(lua_Base* LUA)

#define GMOD_MODULE_CLOSE()                    \
int __gmod13_close(lua_Base* LUA);             \
GMOD_DLL_EXPORT int gmod13_close(lua_State* L) \
{                                              \
    return __gmod13_close(L->luaBase);         \
}                                              \
int __gmod13_close(lua_Base* LUA)

#define LUA_FUNCTION(FUNC)                     \
int __FUNC##(lua_Base* LUA);                   \
int FUNC(lua_State* L)                         \
{                                              \
    lua_Base* LUA = L->luaBase;                \
    LUA->SetState(L);                          \
    return __FUNC##(LUA);                      \
}                                              \
int __FUNC##(lua_Base* LUA)