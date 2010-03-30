#include "Utilities.h"

#include <cmath>

float  round( float num ) {
    return ( num > 0.0 ) ? (float) floor( num + 0.5 ) : (float) ceil( num - 0.5 );
}