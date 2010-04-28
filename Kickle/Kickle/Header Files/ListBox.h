#ifndef LIST_BOX_H
#define LIST_BOX_H

#include "Control.h"
#include "Container.h"

#include <vector>

// Listbox is a "custom container" that allows multiple options to be listed
// and selected. 
class ListBox : public Container {
 public:
  ListBox();
  ListBox( float x, float y, float width, float height,
           sf::Color baseColor=sf::Color( 0,0,0,0 ), float outline=0.0f,
           sf::Color outlineColor=sf::Color( 0, 0, 0, 0 ), 
           sf::Color selectionColor=sf::Color( 0, 0, 0, 0 ),
           float m_selectorSize=0.0f );
  virtual ~ListBox();

  virtual void Update();
  virtual void Render();
  virtual void HandleEvents();
  
  void SetFont( sf::Font font );
  void AddOption( std::string option );

  unsigned int GetSelection();
  void SetSelection( unsigned int selection );
 private:
   sf::Color m_selectionColor;
   float m_selectorSize;

   sf::String m_currentListString;
   std::vector<std::string> m_list; // List of selectables.
   unsigned int m_listOffset; // Offsets the list if it is greater than
                      // possible selections.
   unsigned int m_selection;
   unsigned int m_maxSelections; // Each box can only show n selections given by size.
};

#endif // LIST_BOX_H