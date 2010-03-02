#ifndef LUAAPPFUNCTS_H
#define LUAAPPFUNCTS_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "BasicTypeDefs.h"

namespace LuaApp {

  void RegisterLuaAppFuncts( lua_State *L );


  //Data exposed to Lua
  extern const luaL_Reg luaAppFuncts[];

  //Contains the key values exposed to lua
  //extern const Uint numKeys;
  //extern const int luaKeys[];


  //Functions exposed to Lua
  //In the future this function may check more than one key at a time
  int LuaIsKeyDown( lua_State *L ); //Returns true if a specific key is pressed
  int LuaGetDeltaTime( lua_State* L );
}

#endif