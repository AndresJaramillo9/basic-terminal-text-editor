#include "ECHandlers.h"
#include "ECTextViewImp.h"
#include "ECDocument.h"
#include "ECCursor.h"
#include "ECFormat.h"

// handler implementations

AbstractHandler::AbstractHandler() : _next(nullptr) {}

Handler * AbstractHandler::SetNext(Handler * handler)
{
    this->_next = handler; 
    return handler;
}

void AbstractHandler::Handle(int key)
{
    if (this->_next) {
        _next->Handle(key);
    }
}

void Typing::Handle(int key)
{
    if ((key >= 32 ) && (key <= 126)) {                 
        _docctrl.InsertText(key);
        _cursorctrl.TypeMove();
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else {
        AbstractHandler::Handle(key);
    }
}

void Entering::Handle(int key)
{
    if (key == ENTER) {
        _docctrl.Enter();
        _cursorctrl.NewLineCursor();
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else {
        AbstractHandler::Handle(key);
    }
}

void Deleting::Handle(int key)
{
    if (key == BACKSPACE) {
        _docctrl.RemoveText();
        _cursorctrl.BackSpaceMove();
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else {
        AbstractHandler::Handle(key);
    }
}

void Tracking::Handle(int key)
{
    if (key == CTRL_Y) {                    
        _docctrl.Redo();
        _cursorctrl.Redo();
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else if (key == CTRL_Z) {               
        _docctrl.Undo();
        _cursorctrl.Undo();
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else {
        AbstractHandler::Handle(key);
    }
}

void ArrowKeys::Handle(int key)
{
    if ((key == ARROW_UP) || (key == ARROW_DOWN) || (key == ARROW_LEFT) || (key == ARROW_RIGHT)) {
        _cursorctrl.ArrowKeyMove(key);
        _form.DocDisplay();
        _form.CursorDisplay();
    }
    else {
        AbstractHandler::Handle(key);
    }
}
