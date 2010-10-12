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
}

/****************************
 * Lua Functions
 ***************************/
int StateMachine::LuaLoadPath( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LoadPath" );
    return 0;
  }
  SAFEDELETE( m_runningState );
  m_runningState = new State;
  m_runningState->Init( lua_tostring( L, -1 ));
  return 0;
}


int StateMachine::LuaReset( lua_State *L ) {
  path = m_runningState->GetPath();
  SAFEDELETE( m_runningState );
  m_runningState = new State;
  m_runningState->Init( path ); 
  return 0;
}


int StateMachine::LuaPortal( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to Portal." );
    return 0;
  }
  std::string path = m_runningState->GetPortalPath( lua_tostring( -1 ));
  if ( path != "" ) {
    SAFEDELETE( m_runningState );
    m_runningState = new State;
    m_runningState->Init( path );
  } else {
    LogLuaErr( "No path associated with portal name passed to Portal." );
  }
  return 0;
}


int StateMachine::LuaLogErr( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LuaLogErr." );
    return 0;
  }
  LogLuaErr( lua_tostring( L, -1 ));
  return 0;
}
 

int StateMachine::LuaGetName( lua_State *L ) {
  return m_runningState->LuaGetName( L );
}  

int StateMachine::LuaCreateObject( lua_State *L ) {
  return m_runningState->LuaCreateObject( L );
}

int StateMachine::LuaDestroyObject( lua_State *L ) {
  return m_runningState->LuaDestroyObject( L );
}

int StateMachine::LuaGetObject( lua_State *L ) {
  return m_runningState->LuaGetObject( L );
}

int StateMachine::LuaGetObjects( lua_State *L ) {
  return m_runningState->LuaGetObjects( L );
}

int StateMachine::LuaGetNearestObject( lua_State *L ) {
  return m_runningState->LuaGetNearestObject( L );
}

int StateMachine::LuaGetObjectOnTile( lua_State *L ) {
  return m_runningState->LuaGetObjectOnTile( L );
}

int StateMachine::LuaObjectBlockingTile( lua_State *L ) {
  return m_runningState->LuaObjectBlockingTile( L );
}

int StateMachine::LuaGetTileInfo( lua_State *L ) {
  return m_runningState->LuaGetTileInfo( L );
}

int StateMachine::LuaTileIsCrossable( lua_State *L ) {
  return m_runningState->LuaTileIsCrossable( L );
}

int StateMachine::LuaSetTile( lua_State *L ) {
  return m_runningState->LuaSetTile( L );
}

