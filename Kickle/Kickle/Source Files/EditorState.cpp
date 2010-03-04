#include "EditorState.h"

#include <SFML/Graphics/Font.hpp>

#include "App.h"
#include "PlayState.h"
#include "TitleState.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
EditorState *EditorState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
EditorState::EditorState() {
}


EditorState::~EditorState() {
	CleanUp();
}


BaseState* EditorState::GetInstance() {
	if( !m_instance ) {
		m_instance = new EditorState();
	}
	return m_instance;
}


void EditorState::DestroyInstance() {
	SAFEDELETE( m_instance );
}


void EditorState::Init() {
	SetInit( true );
  App::GetApp()->SetClearColor( sf::Color(0,49,139) );
  
  App::GetApp()->GetConfig()->SetXPad( 0 );
  App::GetApp()->GetConfig()->SetYPad( (App::GetApp()->GetConfig()->GetScreenHeight() - 
                                        App::GetApp()->GetConfig()->GetMapPixelSize()));

  CreateButtons();
  CreateSidePanel();
  
  m_font = new sf::Font();
  m_font->LoadFromFile( "Content/Core/Fonts/MICKEY.TTF" );

  m_saveMap.SetText( sf::String( "Save Map", *m_font, 20 ) );
  m_loadMap.SetText( sf::String( "Load Map", *m_font, 20 ) );
  m_loadTileSheet.SetText( sf::String( "Load Sheet", *m_font, 20 ) );
  m_tileLayer.SetText( sf::String( "Tile", *m_font, 20 ) );
  m_collisionLayer.SetText( sf::String( "Collision", *m_font, 20 ) );
  m_gameObjectLayer.SetText( sf::String( "Object", *m_font, 20 ) );
  m_exit.SetText( sf::String( "Exit", *m_font, 20 ) );


}


void EditorState::CleanUp() {
	SetInit( false );
  App::GetApp()->SetClearColor( sf::Color() );
  SAFEDELETE( m_font );
  /*
  Return the values to their orignal state.
  */
  App::GetApp()->GetConfig()->SetXPad( (App::GetApp()->GetConfig()->GetScreenWidth() - 
                                        App::GetApp()->GetConfig()->GetMapPixelSize())/2 );
  App::GetApp()->GetConfig()->SetYPad( (App::GetApp()->GetConfig()->GetScreenHeight() - 
                                        App::GetApp()->GetConfig()->GetMapPixelSize())/2);
}


void EditorState::Pause() {

}


void EditorState::Resume() {

}


void EditorState::HandleEvents() {
  SideBarEvents();
  ToolbarEvents();
}


void EditorState::Update() {
  m_level.Update();
}


void EditorState::Render() {
  m_level.Render();
  DrawGrid();
  DrawToolbar();
  DrawSideBar();
}

void EditorState::CreateButtons() {
  // Not at all ideal, we need to make a Toolbar class...
  // This is a proof of concept.
  const float EDIT_WIDTH = 100.f;
  const float EDIT_HEIGHT = 35.f;
 
  sf::Vector2f buttonPos( 10.f, 0.f );

  m_saveMap = Button( SaveFile, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));

  buttonPos.x += EDIT_WIDTH + 50.0f ;

  m_loadMap = Button( LoadFile, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));

  buttonPos.x += EDIT_WIDTH + 50.0f ;

   m_loadTileSheet =  Button( LoadTileSheet, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));
    buttonPos.x += EDIT_WIDTH + 50.0f ;

   m_exit =  Button( Exit, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));

   // SideBar Buttons

    buttonPos.x = (float)App::GetApp()->GetConfig()->GetMapPixelSize()-10.0f;
    buttonPos.y = (float)App::GetApp()->GetConfig()->GetYPad();

    m_tileLayer =  Button( TileLayer, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));
    buttonPos.x += EDIT_WIDTH + 10.0f ;

   m_collisionLayer =  Button( CollisionLayer, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));
    buttonPos.x += EDIT_WIDTH + 10.0f ;

   m_gameObjectLayer =  Button( GameObjectLayer, buttonPos, sf::IntRect( 
    static_cast< int >( buttonPos.x ), 
    static_cast< int >( buttonPos.y ), 
    static_cast< int >( buttonPos.x + EDIT_WIDTH ), 
    static_cast< int >( buttonPos.y + EDIT_HEIGHT )
  ));
}

void EditorState::CreateSidePanel() {

}

void EditorState::DrawGrid() {
  for ( float i = 0; i <= Config::MAP_SIZE; i++ ) {
    App::GetApp()->Draw(sf::Shape::Line( i*Config::TILE_SIZE, 
                                        (float)App::GetApp()->GetConfig()->GetYPad(), 
                                        i*App::GetApp()->GetConfig()->GetTileSize(), 
                                        (float)App::GetApp()->GetConfig()->GetMapPixelSize()+
                                        (float)App::GetApp()->GetConfig()->GetYPad(), 
                                        1 ,sf::Color::Black ));

    App::GetApp()->Draw(sf::Shape::Line( 0, i*App::GetApp()->GetConfig()->GetTileSize()+
                                        (float)App::GetApp()->GetConfig()->GetYPad(),
                                        (float)App::GetApp()->GetConfig()->GetMapPixelSize(),
                                        i*App::GetApp()->GetConfig()->GetTileSize()+
                                        (float)App::GetApp()->GetConfig()->GetYPad(), 
                                        1, sf::Color::Black ));
  }
}

void EditorState::DrawSideBar() {
  App::GetApp()->Draw(sf::Shape::Rectangle((float)App::GetApp()->GetConfig()->GetMapPixelSize(),
                                          (float)App::GetApp()->GetConfig()->GetYPad()+5.0f, // The outline size.
                                          (float)App::GetApp()->GetConfig()->GetScreenWidth(), 
                                          (float)App::GetApp()->GetConfig()->GetScreenHeight(),
                                          sf::Color(153,180,209), 5.0f, sf::Color::Black));
  App::GetApp()->Draw( m_tileLayer );
  App::GetApp()->Draw( m_collisionLayer );
  App::GetApp()->Draw( m_gameObjectLayer );
}

void EditorState::SideBarEvents() {
  if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left ) ) {
     if ( m_tileLayer.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_tileLayer.Activate();
    }
    else if ( m_collisionLayer.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_collisionLayer.Activate();
    }
    else if ( m_gameObjectLayer.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_gameObjectLayer.Activate();
    }
  }
}

void EditorState::DrawToolbar() {
  App::GetApp()->Draw(sf::Shape::Rectangle(0.0f, 0.0f,
                                          (float)App::GetApp()->GetConfig()->GetScreenWidth(), 
                                          (float)App::GetApp()->GetConfig()->GetYPad(),
                                          sf::Color(153,180,209), 5.0f, sf::Color::Black));
    // Draw Buttons
  App::GetApp()->Draw( m_saveMap );
  App::GetApp()->Draw( m_loadMap );
  App::GetApp()->Draw( m_loadTileSheet );
  App::GetApp()->Draw( m_exit );
}

void EditorState::ToolbarEvents() {
  if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left ) ) {
     if ( m_saveMap.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_saveMap.Activate();
    }
    else if ( m_loadMap.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_loadMap.Activate();
    }
    else if ( m_loadTileSheet.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_loadTileSheet.Activate();
    }
    else if ( m_exit.ContainsCursor( App::GetApp()->GetInput().GetMouseX(), 
                                App::GetApp()->GetInput().GetMouseY() )) {
      m_exit.Activate();
    }
  }
}

// Button Actions
void EditorState::SaveFile() {
}

void EditorState::LoadFile() {
}

void EditorState::LoadTileSheet() {
}

void EditorState::TileLayer() {
}

void EditorState::CollisionLayer() {
}

void EditorState::GameObjectLayer() {
}

void EditorState::Exit() {
  App::GetApp()->SetNextState( TitleState::GetInstance() );
}