#ifndef DISPATCH_THREAD_H
#define DISPATCH_THREAD_H


#include <rotor/Registry.h>
#include <QThread>


class ApplicationWindow;
class Configuration;


class DispatchThread : public QThread
{
public:
  DispatchThread( Rotor::Registry & registry, ApplicationWindow & window );
  void run();

private:
  Rotor::Registry &     _registry;
  ApplicationWindow &   _window;
  const Configuration & _configuration;
};


#endif //DISPATCH_THREAD_H
