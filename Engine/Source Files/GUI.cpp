#include "GUI.h"

#include "boost/bind/bind.hpp"
extern "C" {
#include "lualib.h"
}
#include "tinyxml.h"

#include "App.h"
#include "GameState.h"
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
  m_input.ScanInput( m_ptrCallScriptFunc );
}


void GUI::Update() {
  m_sprite.Update();
}


void GUI::Render() {
  App::GetApp()->Draw( m_sprite );
}

/******************************
 * Protected Methods
 * ***************************/
GameState* GUI::GetState() const {
  return m_state;
}


int GUI::GetID() const {
  return m_id;
}


AnimSprite& GUI::GetSprite() {
  return m_sprite;
}

/******************************
 * Private Methods
 * ***************************/
bool GUI::LoadData( const std::string &filepath ) {
  TiXmlDocument doc( filepath.c_str() );
  if ( !doc.LoadFile() ) {
    LogErr( "Couldn't load GUI file " + filepath );
    return false;
  }
  TiXmlHandle handleDoc( &doc );
  TiXmlElement *root = handleDoc.FirstChildElement( "GUI" ).Element();

  m_name = GetXmlFileName( filepath );

  TiXmlElement *anim = root->FirstChildElement( "animation" );
  if ( anim ) {
    m_sprite.LoadAnimData( anim->Attribute( "path" ));

    TiXmlElement *script = root->FirstChildElement( "script" );
    if ( script ) {
      m_luaPath = script->Attribute( "path" );
    }

    const TiXmlElement *inputListRoot = root->FirstChildElement( "input_list" );
    if ( inputListRoot ) {
      if ( !m_input.LoadInputList( inputListRoot )) {
        LogErr( "Problem loading input list in GUI" );
        return false;
      }
    }
  } else {
    LogErr( "<animation> element not specified in GUI file." );
    return false;
  }

  return true;
}


void GUI::InitLua() {
  luaL_dofile( App::GetApp()->LuaState(), m_luaPath.c_str() );
  if ( lua_istable( App::GetApp()->LuaState(), -1 )) {
    m_id = luaL_ref( App::GetApp()->LuaState(), LUA_REGISTRYINDEX );
  }
}


void GUI::CallScriptFunc( std::string &funcName ) {
  lua_State *L = App::GetApp()->LuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, m_id );
  lua_getfield( L, -1, funcName.c_str() );
  if( lua_isfunction( L, -1 ) ) {
    lua_call( L, 0, 0 );
  }
  lua_settop( L, 0 );
}

