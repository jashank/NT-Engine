#include "Camera.h"

namespace nt {
namespace scene {

Camera::Camera()
 :  m_tileCulling( true ),
    m_objectCulling( true)  {
}

Camera::~Camera() {
}

bool Camera::GetTileCulling() {
  return m_tileCulling;
}

void Camera::SetTileCulling( bool tileCulling ) {
  m_tileCulling = tileCulling;
}

bool Camera::GetObjectCulling() {
  return m_objectCulling;
}

void Camera::SetObjectCulling( bool objectCulling ) {
  m_objectCulling = objectCulling;
}

} // Namespace nt
} // Namespace scene
