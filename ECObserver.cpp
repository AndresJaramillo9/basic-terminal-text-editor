#include "ECConcreteObserver.h"
#include "ECTextViewImp.h"
#include "ECHandlers.h"

// the one observer's implementation

void Observer::Update()
{
    _key = _view.GetPressedKey();
    _handler.Handle(_key);
} 