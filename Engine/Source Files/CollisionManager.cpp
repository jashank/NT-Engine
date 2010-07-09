#include "CollisionManager.h"

#include <sstream>
#include "tinyxml.h"
#include "App.h"

/************************************************************
 Constructors and Destructors
 ***********************************************************/
CollisionManager::CollisionManager() : m_width(0), m_height(0) {}

/************************************************************
Public Member Functions
************************************************************/
void CollisionManager::LoadData( const TiXmlElement *dataRoot ) {
  if ( !dataRoot->Attribute( "width", &m_width ) ||
       !dataRoot->Attribute( "height", &m_height )) {
    LogErr( "Didn't specify width or height in collision layout." );
  }
  std::string collisionString( dataRoot->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );

  int currentTile = 0;

  int collisionLayout[15][15]; // TODO fix to dynamic

  for( int i=0; i < m_height; i++ ) {
    for ( int j=0; j < m_width; j++ ) {
      if ( collisionMapStream >> currentTile ) {
        collisionLayout[i][j] = currentTile;
      } else {
        collisionLayout[i][j] = -1;
      }
    }
  }

  for ( int i = 0; i < m_height; i++ ) {
    for ( int j = 0; j < m_width; j++ ) {
      ( collisionLayout[i][j] == CROSSABLE ) ?
          m_collisionLayout[i][j] = CROSSABLE :
          m_collisionLayout[i][j] = NOT_CROSSABLE;
    }
  }
}


bool CollisionManager::TileIsCrossable( int x, int y ) const {
  if ( x >= m_width || y >= m_height ) {
    return false;
  } else {
    return m_collisionLayout[y][x] == CROSSABLE;
  }
}


void CollisionManager::SetCollision( int x, int y, int collisionId ) {
  if ( x < m_width && y < m_height ) {
    m_collisionLayout[y][x] = collisionId;
  }
}


int CollisionManager::GetCollision( int x, int y ) {
  if ( x < m_width && y < m_height ) {
    return m_collisionLayout[y][x];
  } else {
    return NOT_CROSSABLE;
  }
}
