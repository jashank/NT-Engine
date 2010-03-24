#ifndef GAME_OBJECT_MAP_H
#define GAME_OBJECT_MAP_H

#include <map>
#include <string>
#include <vector>

#include "App.h"
#include "BasicTypeDefs.h"
#include "GameObject.h"
#include "Utilities.h"
#include "tinyxml.h"

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
  GameObject* ObjectOnTile( Uint x, Uint y );

  // Returns a GameObject in the gameObjectMap with the given 'objectType' 
  // as its type. Returns NULL if an object isn't found.
  GameObject* GetGameObject( const std::string &objectType );

 private:
  void Init();

  // This is a map of ids to game objects
  std::map<int, GameObject*> m_gameObjects;
  // This is a list of dead players ids that need to be reused
  std::vector<int> m_avaliableIds;
  // This is the next possible expansion to the players ids.
  int m_nextId;

  static const int MAP_SIZE = 15;
};

#endif