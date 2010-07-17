#include "Button.h"

#include <SFML/Window/Input.hpp>
#include <SFML/Window/Event.hpp>

extern "C" {
#include "lualib.h"
}
#include "Lunar.h"
#include "tinyxml.h"

#include "App.h"
#include "GameState.h"
#include "Utilities.h"

/*************************************
 * Constructors and Destructors
 * **********************************/
Button::Button( const std::string &filepath, int x, int y )
  : GUI( filepath, x, y ), m_pressedDown( false ) {
  if ( !LoadData( filepath )) {
    LogErr( "Problem loading Button XML file " + filepath );
  }
}

/*************************************
 * Public Methods
 * **********************************/
void Button::HandleEvents() {
  GUI::HandleEvents();

  const std::string MOUSE_PRESSED = "MousePressed";
  const std::string MOUSE_CLICKED = "MouseClicked";
  const std::string MOUSE_RELEASED = "MouseReleased";

  int x = App::GetApp()->GetInput().GetMouseX();
  int y = App::GetApp()->GetInput().GetMouseY();

  if ( m_pressable.Contains( x, y )) {
    sf::Event event = App::GetApp()->GetEvent();

    if ( event.MouseButton.Button == sf::Mouse::Left ) {
      if ( event.Type == sf::Event::MouseButtonPressed ) {
        LuaCallMouseInput( MOUSE_PRESSED );
        m_pressedDown = true;

      } else if ( event.Type == sf::Event::MouseButtonReleased ) {
        if ( m_pressedDown ) {
          LuaCallMouseInput( MOUSE_CLICKED );
        } else {
          LuaCallMouseInput( MOUSE_RELEASED );
        }
      }
    }
  } else {
    m_pressedDown = false;
  }
}

/***************************
 * Private Methods
 * ************************/
bool Button::LoadData( const std::string &filepath ) {
  TiXmlDocument doc( filepath.c_str() );
  if ( !doc.LoadFile() ) {
    LogErr( "Couldn't load GUI Button file: " + filepath );
    return false;
  }
  TiXmlHandle handleDoc( &doc );

  TiXmlElement *root = handleDoc.FirstChildElement( "GUI" ).Element();
  if ( root ) {
    TiXmlElement *rect = root->FirstChildElement( "rect" );
    if ( rect ) {
      rect->Attribute( "x", &m_pressable.Left );
      rect->Attribute( "y", &m_pressable.Top );
      rect->Attribute( "width", &m_pressable.Right );
      rect->Attribute( "height", &m_pressable.Bottom );

      m_pressable.Left += GetSprite().GetPosition().x;
      m_pressable.Top += GetSprite().GetPosition().y;
      m_pressable.Right += m_pressable.Left;
      m_pressable.Bottom += m_pressable.Top;

    } else {
      LogErr( "Pressable rect not defined in Button xml file." );
      return false;
    }
  } else {
    LogErr( "<GUI> tag missing in Button xml file." );
    return false;
  }

  return true;
}


void Button::LuaCallMouseInput( const std::string &inputType  ) {
  lua_State *L = App::GetApp()->GetLuaState();
  lua_rawgeti( L, LUA_REGISTRYINDEX, GetID() );
  lua_getfield( L, -1, inputType.c_str() );
  if ( lua_isfunction( L, -1 )) {
    // Lunar<GUI>::push( L, this );
    lua_call( L, 0, 0 );
  }
  lua_settop( L, 0 );
}
