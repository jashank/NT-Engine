#ifndef GAME_OBJECT_MAP_H
#define GAME_OBJECT_MAP_H

#include <map>
#include <vector>
#include "GameObject.h"

class GameObjectMap {
  public:
    GameObjectMap();
    virtual ~GameObjectMap();
    
    void Init();
    
    void Render();
    bool SetGameObjectMap(/*Take some tinyXML*/);

  private:
    // This is a map of ids to game objects
    std::map<int, GameObject> m_gameObjects;
    // This is a list of dead players ids that need to be reused
    std::vector<int> m_avaliableIds;
    // This is the next possible expansion to the players ids.
    int m_nextId;
};

#endif