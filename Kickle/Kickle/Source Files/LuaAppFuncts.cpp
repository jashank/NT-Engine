#include "LuaAppFuncts.h"

#include <SFML/Window/Input.hpp>

#include "App.h"

namespace LuaApp {

void RegisterLuaAppFuncts( lua_State *L ) {
  //TODO - figure out how to create a table(with keys[i don't mean keyboard keys, but associative array keys) and expose it to Lua
  //lua_createtable( L
  luaL_register( L, "Game", luaAppFuncts );
}


//Data exposed to Lua
const luaL_Reg luaAppFuncts[] = {
  { "IsKeyDown", LuaIsKeyDown },
  { "GetDeltaTime", LuaGetDeltaTime },
  { 0, 0 }
};


//Contains the key values exposed to lua
//const Uint numKeys = 5;
//const int luaKeys[] = {
//  sf::Key::Up,
//  sf::Key::Down,
//  sf::Key::Left,
//  sf::Key::Right,
//  sf::Key::Z
//};


//Functions exposed to Lua


//Returns true if a specific key is pressed
//In the future this function may check more than one key at a time
int LuaIsKeyDown( lua_State *L ) {
  static const App* app = App::GetApp();
  static const sf::Input& inp = app->GetInput();

  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid argument for IsKeyDown." );
  }

  bool result = 
    inp.IsKeyDown( static_cast<sf::Key::Code>( lua_tointeger( L, -1 ) ) );

  lua_pushboolean(L, ( result ) ? 1 : 0 );

  return 1;
}

//Returns time spent processing last frame
int LuaGetDeltaTime( lua_State* L ) {
  App* app = App::GetApp();
  lua_pushnumber( L, app->GetDeltaTime() );
  return 1;
}

}