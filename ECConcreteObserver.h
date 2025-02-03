#ifndef ECCONCRETEOBSERVER_H
#define ECCONCRETEOBSERVER_H

#include "ECObserver.h"
#include "ECHandlers.h"

// the one observer

class Observer : public ECObserver
{
public:
    Observer(ECTextViewImp& view, Handler& handler) : _view(view), _handler(handler) {}
    ~Observer() {}
    void Update();
private:
    ECTextViewImp& _view;
    Handler& _handler;   
    int _key;
};

#endif