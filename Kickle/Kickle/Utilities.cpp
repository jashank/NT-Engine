#include "Utilities.h"

#include <cmath>

int round( float num ) {
    return ( num > 0.0 ) ? floor( num + 0.5 ) : ceil( num - 0.5 );
}