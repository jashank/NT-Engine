#ifndef DIRECTION_H
#define DIRECTION_H

/**
 * Defines directions used in engine and provides utility functions.
 */
namespace nt {
namespace dir {

enum Direction { North, Northeast, East, Southeast,
                 South, Southwest, West, Northwest };

/**
 * Returns true if dir is a cardinal direction
 * (North, East, South, West).
 */
bool IsCardinal( Direction dir );

/**
 * Returns direction opposite to dir passed.
 */
Direction GetOpposite( Direction dir );

} // namespace dir
} // namespace nt

#endif // DIRECTION_H
