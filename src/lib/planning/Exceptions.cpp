#include "Exceptions.h"

//------------------------------------------------------------------------------

NotFoundError::NotFoundError( const std::string & message ) 
: logic_error( message ) {}

