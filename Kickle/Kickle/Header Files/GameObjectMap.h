#ifndef GAME_OBJECT_MAP_H
#define GAME_OBJECT_MAP_H

#include <string>
#include <list>
#include <vector>

#include "tinyxml.h"

#include "App.h"
#include "GameObject.h"
#include "Utilities.h"

class GameObjectMap {
 public:
  static const int NULL_GAME_OBJECT = -1;

  GameObjectMap();
  virtual ~GameObjectMap();
  
  // Update and render the objects in the map.
  void Update(); 
  void Render();

  bool SetGameObjectMap( TiXmlElement* root );

  // Functions to add and remove 'gameObject' passed.
  void AddGameObject( GameObject *gameObject );
  void RemoveGameObject( GameObject *gameObject );
  
  // Collision detection for non-solid GameObjects using rectangular
  // collision detection. Returns GameObject that 'gameObject' collided with,
  // or NULL otherwise.
  GameObject* DetectCollision( const GameObject *gameObject );

  // If an object is on the specified tile location, that object is returned. 
  // Returns NULL otherwise.
  GameObject* ObjectOnTile( unsigned int x, unsigned int y );

  // Returns a GameObject in the gameObjectMap with the given 'objectType' 
  // as its type. Returns NULL if an object isn't found.
  GameObject* GetGameObject( const std::string &objectType );

 private:
  typedef std::list<GameObject*>::iterator GameObjItr;

  // Vector of game objects on map
  std::list<GameObject*> m_gameObjects;
  // Holds GameObjects that were sent to be destroyed on the last update
  std::vector<GameObject*> m_toBeDestroyed;

  static const int MAP_SIZE = 15;
};

#endif