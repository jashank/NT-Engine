#include "CollisionManager.h"

#include <sstream>
#include "tinyxml.h"
#include "App.h"
#include "Config.h"

/****************************
Constructors and Destructors
*****************************/
CollisionManager::CollisionManager( const TiXmlElement *dataRoot ) {
  LoadData( dataRoot );
}

/************************************************************
Public Member Functions
************************************************************/
void CollisionManager::LoadData( const TiXmlElement *dataRoot ) {
  std::string collisionString( dataRoot->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );
  
  int currentTile = 0;

  int collisionLayout[15][15]; // TODO fix to dynamic

  for( unsigned int i=0; i < Config::GetMapSize(); i++ ) {
    for ( unsigned int j=0; j < Config::GetMapSize(); j++ ) {
      if ( collisionMapStream >> currentTile ) {
        collisionLayout[i][j] = currentTile;
      } else {
        collisionLayout[i][j] = -1;
      }
    }
  }

  for ( unsigned int i = 0; i < Config::GetMapSize(); i++ ) {
    for ( unsigned int j = 0; j < Config::GetMapSize(); j++ ) {
      ( collisionLayout[i][j] == CROSSABLE ) ? 
          m_collisionLayout[i][j] = CROSSABLE : 
          m_collisionLayout[i][j] = NOT_CROSSABLE;
    }
  }
}


bool CollisionManager::TileIsCrossable( unsigned int x, unsigned int y ) const {
  if ( !Config::IsTileValid( x, y )) {
    return false;
  } else {
    return m_collisionLayout[y][x] == CROSSABLE;
  }
}


void CollisionManager::SetCollision( unsigned int x, unsigned int y, int collisionId ) {
  if ( Config::IsTileValid( x, y ) ) {
    m_collisionLayout[y][x] = collisionId;
  }
}


int CollisionManager::GetCollision( unsigned int x, unsigned int y ) {
  if ( Config::IsTileValid( x, y ) ) {
    return m_collisionLayout[y][x];
  } else {
    return NOT_CROSSABLE;
  }
}