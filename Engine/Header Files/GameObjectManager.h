#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include <list>
#include <string>
#include <vector>

class TiXmlElement;
class GameObject;

class GameObjectManager {
 public:
  static const int NULL_GAME_OBJECT = -1;

  // Calls LoadData to initialize GameObjectManager
  explicit GameObjectManager( const TiXmlElement *dataRoot );
  ~GameObjectManager();

  // Parses data from <game_objects> section of state file
  void LoadData( const TiXmlElement *dataRoot );
  
  //Handle the events of the GameObjects
  void HandleEvents();
  // Update and render the GameObjects in the manager.
  void Update(); 
  void Render() const;

  // Functions to add and remove 'gameObject' passed.
  void AddGameObject( GameObject *gameObject );
  void RemoveGameObject( GameObject *gameObject );
  
  // Collision detection for non-solid GameObjects using rectangular
  // collision detection. Returns GameObject that 'gameObject' collided with,
  // or NULL otherwise.
  GameObject* DetectCollision( const GameObject *gameObject ) const;

  // If an object is on the specified tile location, that object is returned. 
  // Returns NULL otherwise.
  GameObject* ObjectOnTile( unsigned int x, unsigned int y ) const;

  // Returns a GameObject in the GameObjectManager with the given 'objectType' 
  // as its type. Returns NULL if an object isn't found.
  GameObject* GetGameObject( const std::string &objectType ) const;

 private:
  typedef std::list<GameObject*>::iterator GameObjItr;
  typedef std::list<GameObject*>::const_iterator GameObjItrConst;

  // Restricts copy constructor, and assignment.
  GameObjectManager( const GameObjectManager &manager );
  GameObjectManager& operator=( const GameObjectManager &manager );

  // List of game objects that manager is holding
  std::list<GameObject*> m_gameObjects;
  // Holds GameObjects that were sent to be destroyed on the last update
  std::vector<GameObject*> m_toBeDestroyed;

  static const int MAP_SIZE = 15;
};

#endif