#ifndef ECCURSOR_H
#define ECCURSOR_H   

#include "ECTextViewImp.h"
class ECDocument;
class ECCommandHistory;
class ECFormat;

// cursor 

class ECCursor 
{
public:
    ECCursor();
    ~ECCursor();
    void SetFormatter(ECFormat& format);                    // called in main, sets the formatter object as private attribute
    void SetPlacement(int x);                               // sets placement of cursor for inter-paragraph operations
    int GetPlacement();                                     // get placement x pos 
    int GetCursorx();                                       // get raw x position
    int GetCursory();                                       // get raw y position
    void MoveByTyping();                                    // increment raw x position from typing
    void MoveByBackSpace();                                 // decrement raw x position from backspacing on text
    void Newline();                                         // increment raw y position by entering, must go (0,y+1)
    void LineUp(int x);                                     // update x and y position by going a line up
    void Map(int x, int y);                                 // maps raw cursor position from display cursor position input, proceeds to update
    void Update(int x, int y);                              // set raw cursor position
    void Send();                                            // send to formatter for processing

private:
    int _cursorx;
    int _cursory;
    int _placement;
    ECFormat * _form;
}; 

// cursor controller

class ECCursorController
{
public:
    ECCursorController(ECDocument& doc, ECCursor& cursor, ECCommandHistory& history, ECFormat& format);
    ~ECCursorController();
    void ArrowKeyMove(int key);                             // forwards direction to formatter for processing, doesnt create command object
    void TypeMove();                                        // movement by typing 
    void BackSpaceMove();                                   // movement by backspace, handles cases when raw x = 0
    void NewLineCursor();                                   // newline, must save x at row y for undoing
    bool Undo();                                            // control-z
    bool Redo();                                            // control-y
private:
    ECDocument& _document;
    ECCursor& _cursor;
    ECCommandHistory& _history;
    ECFormat& _form; 
};

#endif 