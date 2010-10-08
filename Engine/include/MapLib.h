#ifndef MAPLIB_H
#define MAPLIB_H


namespace nt {
namespace map {

/**
 * Increments point passed, given a width and a height.
 *
 * First increments x. If x is still less then then the width, then returns.
 * If x has greater than or equal to the width, then x is set to 0 and y is
 * incremented. If both breach width and height, respectively, then both are
 * set to -1.
 */
void IncPoint( int &x, int &y, int width, int height );

} // namespace map
} // namespace nt

#endif // MAPLIB_H

