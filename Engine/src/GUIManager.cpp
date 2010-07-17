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
  if ( component ) {
    do {
      const char *type = component->Attribute( "type" );
      if ( type ) {
        FactoryRef::iterator gui = m_factoryRef.find( ToLowerCase( type ));
        if ( gui != m_factoryRef.end() ) {
          const char *path = component->Attribute( "path" );
          if ( path ) {
            const TiXmlElement *instance =
              component->FirstChildElement( "instance" );
            if ( instance ) {
              do {
                int x, y = 0;
                instance->QueryIntAttribute( "x", &x );
                instance->QueryIntAttribute( "y", &y );
                m_components.push_back( gui->second( path, x, y ));
              } while ( instance = instance->NextSiblingElement( "instance" ));
            } else {
              LogErr( "No instances specified for GUI type in state file." );
              return false;
            }
          } else {
            LogErr( "No path specified for GUI in state file." );
            return false;
          }
        } else {
          LogErr( "Non-existent gui type chosen for GUI in state file." );
          return false;
        }
      } else {
        LogErr( "No type specified for GUI component in state file." );
        return false;
      }
    } while ( component = component->NextSiblingElement( "component" ));
  } else {
    LogErr( "No component specified in <GUI>. Thus, <GUI> not needed." );
    return false;
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
