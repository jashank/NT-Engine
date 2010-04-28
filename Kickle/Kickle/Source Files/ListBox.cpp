#include "ListBox.h"

ListBox::ListBox() 
 : m_listOffset( 0 ) ,
   m_selection( 0 ),
   m_selectorSize( 0 ),
   m_selectionColor( 0, 0, 0, 0 ) {
}

ListBox::~ListBox() {
}

ListBox::ListBox( float x, float y, float width, float height, sf::Color baseColor,
                  float outline, sf::Color outlineColor, sf::Color selectionColor,
                  float m_selectorSize )
 : m_listOffset( 0 ) ,
   m_selection( 0 ),
   m_selectorSize( m_selectorSize ){
  SetX( x );
  SetY( y );
  SetWidth( width );
  SetHeight( height );
  SetBaseColor( baseColor );
  SetOutlineColor( outlineColor );
  SetOutlineSize( outline );
  m_selectionColor = selectionColor;
  m_maxSelections = (unsigned int)GetHeight()/(unsigned int)m_selectorSize;
}


void ListBox::Update() {
}

void ListBox::Render() {
  App::GetApp()->Draw( sf::Shape::Rectangle( GetX(), GetY(), GetX()+GetWidth(),
                       GetY()+GetHeight(), m_baseColor, m_outline,
                       m_outlineColor ) );

  App::GetApp()->Draw( sf::Shape::Rectangle( GetX(), 
                                             GetY()+m_selectorSize*m_selection,
                                             GetX()+GetWidth(), 
                                             GetY()+m_selectorSize+m_selectorSize*m_selection, 
                                             m_selectionColor ) );

  // Draws selectable items
  for ( unsigned int i = 0; i < m_list.size(); i++ ) {
    m_currentListString.SetText( m_list[i] );
    m_currentListString.SetPosition( GetX()+m_outline, (i*m_selectorSize + 10 + GetY()) );
    App::GetApp()->Draw( m_currentListString );
  }

}

void ListBox::HandleEvents() {
  if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left )) {
    int mouseX = App::GetApp()->GetInput().GetMouseX();
    if ( mouseX >= GetX() && mouseX <= GetX()+GetWidth() ) {
      int mouseY = App::GetApp()->GetInput().GetMouseY();
      if ( mouseY >= GetY() && mouseY <= GetY()+GetHeight() ) {
        m_selection =  (unsigned int)((mouseY - GetY()) / m_selectorSize);
      }
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Up ) ) {
    if ( m_selection > 0 ) {
      m_selection--;
    }
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Down ) ) {
    if ( ( m_selection + 1 ) < m_maxSelections ) {
      m_selection++;
    }
  }
}

void ListBox::SetFont( sf::Font font ) {
  m_currentListString.SetFont( font );
}

void ListBox::AddOption( std::string option ) {
  m_list.push_back( option );
}

unsigned int ListBox::GetSelection() {
  return m_selection;
}

void ListBox::SetSelection( unsigned int selection ) {
  m_selection = selection;
}
