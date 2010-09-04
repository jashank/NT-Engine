#ifndef CAMERA_H
#define CAMERA_H

#include "Rect.h"

namespace nt {
namespace scene {

class Camera {
 public:
  /// Constructor Empty.
  Camera();

  /// Destructor Empty.
  ~Camera();
  
  /// Returns m_tileCulling
  bool GetTileCulling();
  /// Sets m_tileCulling to tileCulling
  void SetTileCulling( bool tileCulling );
  /// Gets m_objectCulling
  bool GetObjectCulling();
  /// Sets m_objectCulling to objectCulling
  void SetObjectCulling( bool objectCulling );

  nt::core::IntRect frame;
 private:
  bool  m_tileCulling;
  bool m_objectCulling;
};

} // namespace nt
} // namespace scene

#endif // CAMERA_H

