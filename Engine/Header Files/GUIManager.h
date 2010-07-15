#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <list>
#include <map>
#include <string>

class GUI;
class TiXmlElement;

/***************************************************
 * GUIManager
 * Manages GUI in state (renders and handles events)
 * ************************************************/
class GUIManager {
 public:
  GUIManager(){}
  ~GUIManager();

  // Parses data from <GUI> section of state file.
  // Returns whether load was successful.
  bool LoadData( const TiXmlElement *dataRoot );

  // Handle events of GUI
  void HandleEvents();

  // Updates GUI
  void Update();

  // Render GUI
  void Render() const;

 private:
  typedef GUI* (*CreateGUIFunc)( const std::string &filepath, int x, int y );
  typedef std::map<std::string, CreateGUIFunc> FactoryRef;

  // Restricts copy constructor and assignment.
  GUIManager( const GUIManager &manager );
  GUIManager& operator=( const GUIManager &manager );

  // Creates GUI component passed to template and returns pointer to it
  template<class T>
  static GUI* CreateGUI( const std::string &filepath, int x, int y );

  // Creates the factory reference map
  static FactoryRef CreateFactoryRef();

  std::list<GUI*> m_components; // GUI components currently loaded
  // Reference for factory function for which GUI to create
  static FactoryRef m_factoryRef;
};

#endif
