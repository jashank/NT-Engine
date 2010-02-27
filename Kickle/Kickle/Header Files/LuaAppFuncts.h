#ifndef LUAAPPFUNCTS_H
#define LUAAPPFUNCTS_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "BasicTypeDefs.h"

extern const luaL_Reg luaAppFuncts[]; //List of functions to expose to Lua
void RegisterLuaAppFuncts( lua_State *L );


//Data exposed to Lua
extern const Uint numKeys;  //Contains the size of luaKeys
extern const int luaKeys[]; //Contains the key values exposed to lua

//Functions exposed to Lua
int LuaIsKeyDown( lua_State *L ); //Returns true if a specific key is pressed
int LuaGetKeyEvent( lua_State *L ); //Returns true if any key is pressed
int LuaGetDeltaTime( lua_State *L );//returns time spent processing last frame


#endif