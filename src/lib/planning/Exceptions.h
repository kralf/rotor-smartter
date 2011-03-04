#ifndef RANA_GRAPH_EXCEPTIONS_H
#define RANA_GRAPH_EXCEPTIONS_H


#include <stdexcept>

  class NotFoundError : public std::logic_error {
  public:
    NotFoundError( const std::string & message );
  };


#endif //RANA_GRAPH_EXCEPTIONS_H
