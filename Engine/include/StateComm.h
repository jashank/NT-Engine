#ifndef STATECOMM_H
#define STATECOMM_H

class State;

// Doesn't actually contain State class atm
/**
 * Contains State class and and interface to facilitate communication amongst
 * its components. In short, once a State is set to communicate, the functions
 * in this interface will be referring to the components of the State (Tiles,
 * Objects, Sound, etc.).
 */
namespace nt {
namespace state {

/**
 * Sets up communications for State passed. If there is already a State using
 * the interface, returns false and no action is taken.
 */
bool SetStateComm( State *state );

/**
 * Ends communications for current State (enabling another State to enable
 * communication).
 */
void EndStateComm();

/**
 * Returns true if tile at location passed is crossable and has no Objects
 * blocking access to it. Does check to see if tile is valid.
 */
bool TileIsOpen( int x, int y );

} // namespace state
} // namespace nt

#endif // STATECOMM_H
