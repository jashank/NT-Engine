#include "GUIManager.h"

#include "tinyxml.h"

#include "Button.h"
#include "GUI.h"
#include "Image.h"
#include "Utilities.h"

/*******************************
 * Static Members
 * ****************************/
GUIManager::FactoryRef GUIManager::m_factoryRef = CreateFactoryRef();

/*******************************
 * Constructors and Destructors
 * ****************************/
GUIManager::GUIManager( const TiXmlElement *dataRoot ) {
  if ( !LoadData( dataRoot )) {
    LogErr( "Problem loading GUI in state file." );
  }
}


GUIManager::~GUIManager() {
  for ( std::list<GUI*>::iterator gui = m_components.begin();
        gui != m_components.end(); ++gui ) {
    SAFEDELETE( *gui );
  }
}

/*******************************
 * Public Methods
 * ****************************/
bool GUIManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *component = dataRoot->FirstChildElement( "component" );

  while ( component ) {
    std::string type = component->Attribute( "type" );
    ToLowerCase( type );

    FactoryRef::iterator gui = m_factoryRef.find( type );
    if ( gui != m_factoryRef.end() ) {
      std::string path = component->Attribute( "path" );

      const TiXmlElement *instance = component->FirstChildElement( "instance" );
      do {
        int x, y = 0;
        instance->Attribute( "x", &x );
        instance->Attribute( "y", &y );

        m_components.push_back(gui->second( path, x, y ));
      } while ( instance = instance->NextSiblingElement( "instance" ));
    } else {
      LogErr( "Non-existent gui type chosen for GUI" );
      return false;
    }
    component = component->NextSiblingElement( "component" );
  }
  return true;
}


void GUIManager::HandleEvents() {
  for ( std::list<GUI*>::iterator gui = m_components.begin();
        gui != m_components.end(); ++gui ) {
    (*gui)->HandleEvents();
  }
}


void GUIManager::Update() {
  for ( std::list<GUI*>::const_iterator gui = m_components.begin();
        gui != m_components.end(); ++gui ) {
    (*gui)->Update();
  }
}


void GUIManager::Render() const {
  for ( std::list<GUI*>::const_iterator gui = m_components.begin();
        gui != m_components.end(); ++gui ) {
    (*gui)->Render();
  }
}

/******************************
 * Private Methods
 * ***************************/
template<class T>
GUI* GUIManager::CreateGUI( const std::string &filepath, int x, int y ) {
  return new T( filepath, x, y );
}


GUIManager::FactoryRef GUIManager::CreateFactoryRef() {
  FactoryRef map;
  const std::string image = "image";
  const std::string button = "button";
  map.insert( FactoryRef::value_type( image, &CreateGUI<Image>));
  map.insert( FactoryRef::value_type( button, &CreateGUI<Button>));

  return map;
}
