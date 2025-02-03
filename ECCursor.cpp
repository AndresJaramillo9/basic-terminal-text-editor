#include "ECDocument.h"
#include "ECFormat.h"
using namespace std;

// cursor controller implementation

ECCursorController::ECCursorController(ECDocument& doc, ECCursor& cursor, ECCommandHistory& history, ECFormat& format) : 
    _document(doc), _cursor(cursor), _history(history), _form(format) {}

ECCursorController::~ECCursorController() {}

void ECCursorController::ArrowKeyMove(int key)
{
    if (key == ARROW_UP) {
        _form.ArrowKeyDisplay('u');
    }
    else if (key == ARROW_DOWN) {
        _form.ArrowKeyDisplay('d');
    }
    else if (key == ARROW_LEFT) {
        _form.ArrowKeyDisplay('l');
    }
    else if (key == ARROW_RIGHT) {
        _form.ArrowKeyDisplay('r');
    }
}

void ECCursorController::TypeMove() 
{
    int cursorx = _cursor.GetCursorx();
    int cursory = _cursor.GetCursory();                                  
    ECCommand * cmd = new ::TypeMove(cursorx, cursory, _cursor);
    _history.ExecuteCommand(cmd);
}

void ECCursorController::BackSpaceMove()
{
    int cursorx = _cursor.GetCursorx();
    int cursory = _cursor.GetCursory();
    if (cursorx != 0 || cursory != 0) {
        if (cursorx == 0) {
            if (_document.WindowValidMove(cursorx, cursory-1)) {
                int placement = _cursor.GetPlacement();
                ECCommand * cmd = new ::CursorUp(placement, _cursor);
                _history.ExecuteCommand(cmd);
            }
        }
        else {
            ECCommand * cmd = new ::BackSpaceMove(_cursor);
            _history.ExecuteCommand(cmd);
        }
    }
}

void ECCursorController::NewLineCursor()
{
    int placement = _document.GetRowSize(_cursor.GetCursory());         
    ECCommand * cmd = new ::NewLineCursor(placement, _cursor);
    _history.ExecuteCommand(cmd);
}

bool ECCursorController::Undo()
{
    if (_history.Undo()) {
        return true;
    }
    else {
        return false;
    }
}

bool ECCursorController::Redo()
{
    if (_history.Redo()) {
        return true;
    }
    else {
        return false;
    }
}


// cursor implementation


ECCursor::ECCursor() : _cursorx(0), _cursory(0) {}

ECCursor::~ECCursor() {}

void ECCursor::SetFormatter(ECFormat& format) { _form = &format;}

void ECCursor::SetPlacement(int x) {_placement = x;}

int ECCursor::GetPlacement() {return _placement;}

int ECCursor::GetCursory() { return _cursory;}

int ECCursor::GetCursorx() { return _cursorx;}

void ECCursor::MoveByTyping()
{
    _cursorx += 1;
    Send();
}

void ECCursor::MoveByBackSpace() 
{ 
    _cursorx -= 1;
    Send();
}

void ECCursor::Newline()
{
    _cursorx = 0;
    _cursory += 1;
    Send();
}

void ECCursor::LineUp(int x)
{
    if (_cursory != 0) {
        _cursory -= 1;
        _cursorx = x;
        Send();
    }
}

void ECCursor::Map(int x, int y)
{
    _cursorx = x;
    _cursory = y;
}

void ECCursor::Update(int x, int y)
{
    _cursorx = x;
    _cursory = y;
    Send();
}

void ECCursor::Send() { _form->CurRawCursor(_cursorx, _cursory);}
