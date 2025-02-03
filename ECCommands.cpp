#include "ECCommands.h"
#include "ECDocument.h"
#include "ECFormat.h"
using namespace std;

// history implementation

ECCommandHistory::ECCommandHistory() : _pos(0) {}                   

ECCommandHistory::~ECCommandHistory() 
{
    for (auto cmd : _commands) {                                       
        delete cmd;
    }
}

bool ECCommandHistory::Undo()                       
{
    if (_pos < 0 || _commands.size() == 0) {                        
        return false;
    }
    else {
        ECCommand * cmd = _commands[_pos];
        cmd->Unexecute();                                           
        _pos--;                                                     
    }
    return true;
}

bool ECCommandHistory::Redo()                 
{ 
    int size = _commands.size();
    if (_pos >= size-1) {                                            
        return false;
    }
    else {
        ECCommand * cmd = _commands[_pos + 1];                       
        _pos++;                                                     
        cmd->Execute();                                             
    }
    return true;
}

void ECCommandHistory::ExecuteCommand(ECCommand * pCmd)        
{
    int size = _commands.size();
    if (size == 0) {
        _commands.push_back(pCmd);   
    }                               
    else {
        auto position = _commands.begin() + _pos + 1;               
        _commands.insert(position, pCmd);                   
        _pos++;                                                     
    }
    pCmd->Execute();                                                
}


// command implementations for cursor 


TypeMove::TypeMove(int cx, int cy, ECCursor& cursor) : _x(cx), _y(cy), _cursor(cursor) {}               

TypeMove::~TypeMove() {}

void TypeMove::Execute() {_cursor.MoveByTyping();}

void TypeMove::Unexecute() {_cursor.Update(_x, _y);}

BackSpaceMove::BackSpaceMove(ECCursor& cursor) : _cursor(cursor)            
{
    _x = _cursor.GetCursorx();
    _y = _cursor.GetCursory();
}

BackSpaceMove::~BackSpaceMove() {}

void BackSpaceMove::Execute() {_cursor.MoveByBackSpace();}

void BackSpaceMove::Unexecute() {_cursor.Update(_x, _y);}

NewLineCursor::NewLineCursor(int cx, ECCursor& cursor) : _x(cx), _cursor(cursor) 
{
    _y = _cursor.GetCursory();
}

NewLineCursor::~NewLineCursor() {}

void NewLineCursor::Execute() {_cursor.Newline();}

void NewLineCursor::Unexecute() {_cursor.Update(_x, _y);}

CursorUp::CursorUp(int cx, ECCursor& cursor) : _x(cx), _cursor(cursor) 
{
    _y = _cursor.GetCursory();
}

CursorUp::~CursorUp() {}

void CursorUp::Execute() {_cursor.LineUp(_x);}

void CursorUp::Unexecute() {_cursor.Update(0, _y);} 


// command implementations for document


InsertText::InsertText(int cx, int cy, int key, ECDocument& document) : _x(cx), _y(cy), _key(key), _doc(document) {}  

InsertText::~InsertText() {}

void InsertText::Execute() {_doc.InsertCharAt(_x, _y, _key);}

void InsertText::Unexecute() {_doc.RemoveCharAt(_x, _y);}

RemoveText::RemoveText(int cx, int cy, ECDocument& document) : _x(cx), _y(cy), _doc(document) {}     

RemoveText::~RemoveText() {}

void RemoveText::Execute()
{
    _key = _doc.GetCharAt(_x-1, _y);
    _doc.RemoveCharAt(_x-1, _y);
}

void RemoveText::Unexecute() {_doc.InsertCharAt(_x-1, _y, _key);}

LineRemoval::LineRemoval(int cy, ECDocument& document) : _y(cy), _doc(document) {} 

LineRemoval::~LineRemoval() {}

void LineRemoval::Execute() {_doc.RemoveLine(_y);}

void LineRemoval::Unexecute() {_doc.Newline(_y-1);} 

BackSpaceFrontChar::BackSpaceFrontChar(int cx, int cy, string chars, ECDocument& document, ECFormat& format) : 
    _x(cx), _y(cy), _chars(chars), _doc(document), _form(format) 
{
    _placement = _doc.GetRowSize(_y-1);
}  

BackSpaceFrontChar::~BackSpaceFrontChar() {}

void BackSpaceFrontChar::Execute()
{
    _doc.InsertStringAt(_placement, _y-1, _chars);
    _doc.RemoveLine(_y);
    _form.Repair(_y-1);
}

void BackSpaceFrontChar::Unexecute()
{
    _doc.RemoveStringAt(_placement, _y-1);
    _doc.Newline(_y-1);
    _doc.InsertStringAt(_x,_y, _chars);
    _form.Repair(_y-1);         
    _form.Repair(_y);
}

NewLineNoChars::NewLineNoChars(int cy, ECDocument& document) : _y(cy), _doc(document) {}  

NewLineNoChars::~NewLineNoChars() {}

void NewLineNoChars::Execute() {_doc.Newline(_y);}

void NewLineNoChars::Unexecute() {_doc.RemoveLine(_y+1);}

NewLineChars::NewLineChars(int cx, int cy, string chars, ECDocument& document, ECFormat& format) : 
    _x(cx), _y(cy), _chars(chars), _doc(document), _form(format) {} 

NewLineChars::~NewLineChars() {}

void NewLineChars::Execute()
{    
    _doc.Newline(_y);
    _doc.RemoveStringAt(_x, _y);
    _doc.InsertStringAt(0, _y+1, _chars);
    _form.Repair(_y);
    _form.Repair(_y+1);
}

void NewLineChars::Unexecute()
{
    _doc.InsertStringAt(_x, _y, _chars);
    _doc.RemoveLine(_y+1);
    _form.Repair(_y);
}
