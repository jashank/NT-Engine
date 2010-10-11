#include "StateMachine.h"

#include <SFML/Window/Event.hpp>

extern "C" {
  #include "lua.h"
  #include "lualib.h"
}

#include "Object.h" // To register Objects to Lua
#include "State.h"
#include "Window.h"
#include "Utilities.h"

/*********************************
 * Lua State API
 ********************************/
const luaL_Reg State::m_luaFuncs[] = {
  { "LoadPath", LuaLoadPath },
  { "Reset", LuaReset },
  { "Portal", LuaPortal },
  { "GetName", LuaGetName },
  { "LogErr", LuaLogErr },
  { "CreateObject", LuaCreateObject },
  { "DestroyObject", LuaDestroyObject },
  { "GetObject", LuaGetObject },
  { "GetObjects", LuaGetObjects },
  { "GetNearestObject", LuaGetNearestObject },
  { "GetObjectOnTile", LuaGetObjectOnTile },
  { "ObjectBlockingTile", LuaObjectBlockingTile },
  { "GetTileInfo", LuaGetTileInfo },
  { "TileIsCrossable", LuaTileIsCrossable },
  { "SetTile", LuaSetTile },
  { NULL, NULL }
};


/*******************************
 * Constructors and Destructors
 ******************************/
~StateMachine::StateMachine() {
  SAFEDELETE( m_runningState );
} 

/*******************************
 * Public Member Functions
 ******************************/
bool StateMachine::Setup( const std::string &filePath ) {
  if ( !( m_luaState = luaL_newstate() )) {
    LogErr( "Couldn't create a lua state. Memory allocation error." );
    return false;
  }
  luaL_openlibs( m_luaState );
  luaL_register( m_luaState, "State", m_luaFuncs );
  Object::LuaRegister( m_luaState );

  m_runningState = new State();
  if ( !m_runningState->Init( filePath )) {
    SAFEDELETE( m_runningState );
    return false;
  }
  return true;
}


// Should a goal be to move all nt::window stuff out of StateMachine?
void StateMachine::Step() {
  m_runningState->HandleEvents();

  static sf::Event closeTest;
  while ( nt::window::GetEvent( closeTest ) ) {
    if ( closeTest.Type == sf::Event::Closed ) {
      nt::window::Close();
    }
  }

  m_runningState->Update();
  m_runningState->Render();

  nt::window::Refresh();
}
