#include "LuaAppFuncts.h"

#include <SFML/Window/Input.hpp>

#include "App.h"

int LuaApp::LuaGetKeyEvent() {
  enum LuaKey { UP, DOWN, LEFT, RIGHT, Z };

  const sf::Input& inp = App::GetApp()->GetInput();
  
  if ( inp.IsKeyDown( sf::Key::Up ) ) {
    return UP;
  }
    else if ( inp.IsKeyDown( sf::Key::Down ) ) { 
    return DOWN;
  } 
    else if ( inp.IsKeyDown( sf::Key::Left ) ) {
    return LEFT;
  }
    else if ( inp.IsKeyDown( sf::Key::Right ) ) {
    return RIGHT;
  }
    else if ( inp.IsKeyDown( sf::Key::Z ) ) {
    return Z;
  }
    else {
    return -1;
  }
}

//Returns time spent processing last frame
int LuaApp::LuaGetDeltaTime( lua_State* L ) {
  App* app = App::GetApp();
  lua_pushnumber( L, app->GetDeltaTime() );
  return 1;
}