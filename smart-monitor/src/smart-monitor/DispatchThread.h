#ifndef DISPATCH_THREAD_H
#define DISPATCH_THREAD_H


#include <rotor/Registry.h>
#include <QThread>


class ApplicationWindow;


class DispatchThread : public QThread
{
public:
  DispatchThread( Rotor::Registry & registry, ApplicationWindow & window );
  void run();

private:
  Rotor::Registry &   _registry;
  ApplicationWindow & _window;
};


#endif //DISPATCH_THREAD_H
