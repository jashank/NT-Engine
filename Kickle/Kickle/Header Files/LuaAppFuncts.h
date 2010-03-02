#ifndef LUAAPPFUNCTS_H
#define LUAAPPFUNCTS_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "BasicTypeDefs.h"

namespace LuaApp {
  //Functions exposed to Lua
  int LuaGetKeyEvent(); // Returns integer value for key event
  int LuaGetDeltaTime( lua_State *L );//returns time spent processing last frame
}


#endif