#include "StateMachine.h"

#include <SFML/Window/Event.hpp>

extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}

#include "Object.h" // To register Objects to Lua
#include "ResourceLib.h"
#include "State.h"
#include "Utilities.h"
#include "Window.h"

/*****************
 * Static Members
 ****************/
const luaL_Reg StateMachine::m_luaStateFuncs[] = {
  { "LoadPath", LuaLoadPath },
  { "Reset", LuaReset },
  { "Portal", LuaPortal },
  { "GetName", LuaGetName },
  { NULL, NULL }
};

const luaL_Reg StateMachine::m_luaMapFuncs[] = {
  { "CreateObject", LuaCreateObject },
  { "DestroyObject", LuaDestroyObject },
  { "GetObject", LuaGetObject },
  { "GetObjects", LuaGetObjects },
  { "GetNearestObject", LuaGetNearestObject },
  { "GetNearestToObject", LuaGetNearestToObject },
  { "GetObjectOnTile", LuaGetObjectOnTile },
  { "GetObjectsOnTile", LuaGetObjectsOnTile },
  { "ObjectBlockingTile", LuaObjectBlockingTile },
  { "GetTileInfo", LuaGetTileInfo },
  { "TileIsCrossable", LuaTileIsCrossable },
  { "SetTile", LuaSetTile },
  { NULL, NULL }
};

const luaL_Reg StateMachine::m_luaMusicFuncs[] = {
  { "Play", LuaPlayMusic },
  { "Stop", LuaStopMusic },
  { "Pause", LuaPauseMusic },
  { "IsPlaying", LuaMusicIsPlaying },
  { "Loop", LuaLoopMusic },
  { "GetVolume", LuaGetMusicVolume },
  { "SetVolume", LuaSetMusicVolume },
  { "AdjustVolume", LuaAdjustMusicVolume },
  { NULL, NULL }
};

const luaL_Reg StateMachine::m_luaCamFuncs[] = {
  { "Span", LuaSpanCam },
  { "Offset", LuaOffsetCam },
  { "Center", LuaCenterCam },
  { "SetSpeed", LuaSetCamSpeed },
  { "AdjustSpeed", LuaAdjustCamSpeed },
  { NULL, NULL }
};

const luaL_Reg StateMachine::m_luaErrorFuncs[] = {
  { "LogErr", LuaLogErr },
  { NULL, NULL }
};

bool StateMachine::m_nextStateSet = false;
boost::scoped_ptr<State> StateMachine::m_runningState;
lua_State *StateMachine::m_luaState = NULL;
std::string StateMachine::m_nextStatePath = "";

/*******************************
 * Constructors and Destructors
 ******************************/
StateMachine::~StateMachine() {
  // Objects need to unreference from lua
  m_runningState.reset();

  lua_close( m_luaState );
  m_luaState = NULL;
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
  luaL_register( m_luaState, "State", m_luaStateFuncs );
  luaL_register( m_luaState, "Map", m_luaMapFuncs );
  luaL_register( m_luaState, "Music", m_luaMusicFuncs );
  luaL_register( m_luaState, "Camera", m_luaCamFuncs );
  luaL_register( m_luaState, "Error", m_luaErrorFuncs );
  Object::LuaRegister( m_luaState );

  m_runningState.reset( new State());
  if ( !m_runningState->Init( filePath, m_luaState )) {
    m_runningState.reset();
    return false;
  }
  return true;
}


// Should a goal be to move all nt::window stuff out of StateMachine?
void StateMachine::Step( float dt ) {
  m_runningState->HandleEvents();

  static sf::Event closeTest;
  while ( nt::window::GetEvent( closeTest ) ) {
    if ( closeTest.Type == sf::Event::Closed ) {
      nt::window::Close();
    }
  }

  m_runningState->Update( dt );

  if ( m_nextStateSet ) {
    NextState();
    m_nextStateSet = false;
  }
}


void StateMachine::Render( float alpha ) {
  m_runningState->Render( alpha );
}

/****************************
 * Lua Functions
 ***************************/
int StateMachine::LuaLoadPath( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LoadPath" );
    return 0;
  }
  m_nextStateSet = true;
  m_nextStatePath = lua_tostring( L, -1 );
  return 0;
}


int StateMachine::LuaReset( lua_State *L ) {
  m_nextStateSet = true;
  m_nextStatePath = m_runningState->GetPath();
  return 0;
}


int StateMachine::LuaPortal( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to Portal." );
    return 0;
  }
  std::string port = lua_tostring( L, -1 );
  std::string path = m_runningState->GetPortalPath( port );
  if ( path != "" ) {
    m_nextStateSet = true;
    m_nextStatePath = path;
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

int StateMachine::LuaGetNearestToObject( lua_State *L ) {
  return m_runningState->LuaGetNearestToObject( L );
}

int StateMachine::LuaGetObjectOnTile( lua_State *L ) {
  return m_runningState->LuaGetObjectOnTile( L );
}

int StateMachine::LuaGetObjectsOnTile( lua_State *L ) {
  return m_runningState->LuaGetObjectsOnTile( L );
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

int StateMachine::LuaPlayMusic( lua_State *L ) {
  return m_runningState->LuaPlayMusic( L );
}

int StateMachine::LuaStopMusic( lua_State *L ) {
  return m_runningState->LuaStopMusic( L );
}

int StateMachine::LuaPauseMusic( lua_State *L ) {
  return m_runningState->LuaPauseMusic( L );
}

int StateMachine::LuaMusicIsPlaying( lua_State *L ) {
  return m_runningState->LuaMusicIsPlaying( L );
}

int StateMachine::LuaLoopMusic( lua_State *L ) {
  return m_runningState->LuaLoopMusic( L );
}

int StateMachine::LuaGetMusicVolume( lua_State *L ) {
  return m_runningState->LuaGetMusicVolume( L );
}

int StateMachine::LuaSetMusicVolume( lua_State *L ) {
  return m_runningState->LuaSetMusicVolume( L );
}

int StateMachine::LuaAdjustMusicVolume( lua_State *L ) {
  return m_runningState->LuaAdjustMusicVolume( L );
}

int StateMachine::LuaSpanCam( lua_State *L ) {
  return m_runningState->LuaSpanCam( L );
}

int StateMachine::LuaOffsetCam( lua_State *L ) {
  return m_runningState->LuaOffsetCam( L );
}

int StateMachine::LuaCenterCam( lua_State *L ) {
  return m_runningState->LuaCenterCam( L );
}

int StateMachine::LuaSetCamSpeed( lua_State *L ) {
  return m_runningState->LuaSetCamSpeed( L );
}

int StateMachine::LuaAdjustCamSpeed( lua_State *L ) {
  return m_runningState->LuaAdjustCamSpeed( L );
}

/***************************
 * Private Member Functions
 **************************/
void StateMachine::NextState() {
  m_runningState.reset( new State());
  m_runningState->Init( m_nextStatePath, m_luaState );
  nt::rsrc::ReleaseUnused();
}
