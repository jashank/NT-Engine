#ifndef NT_DIRECTION_H
#define NT_DIRECTION_H

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

/**
 * Returns true if dir is Northward (North, Northeast, etc.)
 */
bool IsNorthward( Direction dir );

/**
 * Returns true if dir is Southward (South, Southwest, etc.)
 */
bool IsSouthward( Direction dir );

/**
 * Returns true if dir is Westward (West, Northwest, etc.)
 */
bool IsWestward( Direction dir );

/**
 * Returns true if dir is Eastward (East, Southeast, etc.)
 */
bool IsEastward( Direction dir );

} // namespace dir
} // namespace nt

#endif // NT_DIRECTION_H
