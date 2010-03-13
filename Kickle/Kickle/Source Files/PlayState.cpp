#include "PlayState.h"

#include "App.h"
#include "GameObject.h"
#include "Configuration.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
PlayState *PlayState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
PlayState::PlayState() {}


PlayState::~PlayState() {
	CleanUp();
}


BaseState* PlayState::GetInstance() {
	if( !m_instance ) {
		m_instance = new PlayState();
	}
	return m_instance;
}


void PlayState::DestroyInstance() {
	SAFEDELETE( m_instance );
}


void PlayState::Init() {
	SetInit( true );
	App::GetApp()->SetClearColor( sf::Color(0,49,139) );
	// Create a level with a place holder path.
	m_level.SetLevel( "Content/Core/Levels/TestLevel.xml" );
  GameObject::AssignLevel( &m_level );
}


void PlayState::CleanUp() {
	SetInit( false );
}


void PlayState::Pause() {

}


void PlayState::Resume() {

}


void PlayState::HandleEvents() {
  // Does nothing
}


void PlayState::Update() {
  m_level.Update();
}


void PlayState::Render() {
	// Render the map.
	m_level.Render();
}


Level& PlayState::GetLevel() {
  return m_level;
}


int PlayState::LuaCreateGameObject( lua_State *L ) {
  if( !lua_isstring( L, -3 ) ) {
    return luaL_error( L, "Invalid filepath for CreateGameObject." );
  }
  std::string path = lua_tostring( L, -3 );
  //lua_pop( L, -1 );
  
  if( !lua_isnumber( L, -2 ) ) {
    return luaL_error( L, "Invalid tile x position for CreateGameObject." );
  }
  Uint tileX = static_cast<Uint>( lua_tointeger( L, -2 ) );

  if( !lua_isnumber( L, -1 ) ) {
    return luaL_error( L, "Invalid tile y position for CreateGameObject." );
  }
  Uint tileY = static_cast<Uint>( lua_tointeger( L, -1 ) );

  m_instance->m_level.AddGameObject(
    new GameObject( path, tileX, tileY )
  );

  return 0;
}


void PlayState::RegisterLuaPlayFuncts( lua_State *L ) {
  luaL_register( L, "Game", luaPlayFuncts );
}


/************************************************
Private Member Functions
************************************************/

const luaL_Reg PlayState::luaPlayFuncts[] = {
  { "CreateGameObject", LuaCreateGameObject },
  { 0, 0 }
};


