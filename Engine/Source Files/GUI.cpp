#include "GUI.h"

#include "boost/bind/bind.hpp"
extern "C" {
#include "lualib.h"
}
#include "tinyxml.h"

#include "App.h"
#include "GameState.h"
#include "Lunar.h"
#include "Utilities.h"

/********************************
Public Members
*********************************/
const char GUI::className[] = "GUI";

/********************************
 * Static Members
 * *****************************/
GameState* GUI::m_state = NULL;

/********************************
 * Constructor and Destructor
 * *****************************/
GUI::GUI( const std::string &filepath, int x, int y )
 : m_ptrCallScriptFunc( boost::bind( &GUI::CallScriptFunc, this, _1 )) {
  m_state = App::GetApp()->GetCurrentState();

  if ( !LoadData( filepath )) {
    LogErr( "Problem loading GUI xml file: " + filepath );
  } else {
    m_sprite.SetPosition( x, y );
  }

  InitLua();
}


GUI::~GUI() {}

/*******************************
 * Public Methods
 * ****************************/
void GUI::HandleEvents() {
  m_input.ScanEvents( m_ptrCallScriptFunc );
}


void GUI::Render() {
  m_sprite.Update();
}


/******************************
 * Private Methods
 * ***************************/
bool GUI::LoadData( const std::string &filepath ) {
  TiXmlDocument doc( filepath.c_str() );
  if ( !doc.LoadFile() ) {
    return false;
  }
  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "GUI" ).Element();

  m_name = GetXmlFileName( filepath );

  TiXmlElement *anim = handleDoc.FirstChildElement( "animation" ).Element();
  m_sprite.LoadAnimData( anim->Attribute( "path" ));

  TiXmlElement *script = handleDoc.FirstChildElement( "script" ).Element();
  m_luaPath = script->Attribute( "path" );

  const TiXmlElement *inputListRoot = root->FirstChildElement( "input_list" );
  if ( inputListRoot ) {
    if ( !m_input.LoadInputList( inputListRoot )) {
      LogErr( "Problem loading input list in GUI" );
      return false;
    }
  }
  return true;
}


void GUI::InitLua() {
  luaL_dofile( m_state->GetLuaState(), m_luaPath.c_str() );
  if ( lua_istable( m_state->GetLuaState(), -1 )) {
    m_id = luaL_ref( m_state->GetLuaState(), LUA_REGISTRYINDEX );
  } else {
    LogErr( "Lua event table not found for GUI: " + m_name );
  }
}


void GUI::CallScriptFunc( std::string &funcName ) {
  lua_State *L = m_state->GetLuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  lua_getfield( L, -1, funcName.c_str() );
  if( lua_isfunction( L, -1 ) ) {
    Lunar<GUI>::push( L, this );
    lua_call( L, 1, 0 );
  }
  lua_settop( L, 0 );
}

