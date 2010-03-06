#ifndef GAME_OBJECT_MAP_H
#define GAME_OBJECT_MAP_H

#include <map>
#include <vector>

#include "GameObject.h"
#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

class GameObjectMap {
  public:
    GameObjectMap();
    virtual ~GameObjectMap();
    
    void Update(); 
    void Render();

    bool SetGameObjectMap( TiXmlElement* root );

    void AddGameObject( GameObject *gameObject );
    void RemoveGameObject( GameObject *gameObject );

    GameObject *GetGameObject( unsigned int x, unsigned int y ) const;
  private:
    void Init();

    // This is a map of ids to game objects
    std::map<int, GameObject*> m_gameObjects;
    // This is a list of dead players ids that need to be reused
    std::vector<int> m_avaliableIds;
    // This is the next possible expansion to the players ids.
    int m_nextId;

    static const int NULL_GAME_OBJECT = -1;
    static const int MAP_SIZE = 15;

    int m_gameObjectLayout[MAP_SIZE][MAP_SIZE];
};

#endif