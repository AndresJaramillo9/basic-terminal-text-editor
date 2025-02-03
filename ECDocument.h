#ifndef ECDOCUMENT_H
#define ECDOCUMENT_H

#include "ECTextViewImp.h"
#include "ECCommands.h"
#include <vector>
#include <string>
class ECCursor;
class ECFormat;
using namespace std;

// document

class ECDocument
{
public:
    ECDocument(ECTextViewImp& view, vector<string> textBuffer);
    ~ECDocument();
    char GetCharAt(int x, int y);                           // gets char at raw (x,y)
    string GetStringAt(int x, int y);                       // gets remainder of string at raw (x,y), could be whole line
    string GetWordAtEnd(int x, int y);                      // used by strategy object to get partial word for line breaking
    vector<string> GetText();                               // outputs the raw text where lines are paragraphs 
    int GetRowSize(int y);                                  // get length of string at raw row y
    int GetNumRows();                                       // get number of raw rows y
    int GetMaxRowLength();                                  // get row with max length for smart cursor movements
    void InsertCharAt(int x, int y, char letter);           // insert character at raw (x,y)
    void RemoveCharAt(int x, int y);                        // remove character at raw (x,y)
    void InsertStringAt(int x, int y, string text);         // insert text at raw (x,y)
    void RemoveStringAt(int x, int y);                      // remove remainder of text at raw (x,y), could be whole 
    void Newline(int y);                                    // add raw y-row, creates new paragraph in the formatter 
    void RemoveLine(int y);                                 // delete raw y-row, removes corresponding paragraph in formatter
    bool DocValidMove(int x, int y);                        // checks validity of cursor movements left and right
    bool CursorValidMove(int x, int y);                     // checks validity of cursor movements for up and down movement 
    bool WindowValidMove(int x, int y);                     // checks validity of a text or cursor movement with respect to space on view window
    bool EmptyLine(int x, int y);                           // checks if remainder of line has no characters, could be whole 
    void SetFormatter(ECFormat& format);                    // sets the formatter object aas private attribute
    void Send(int y);                                       // send raw document changes to formatter for processing
    
private:
    ECTextViewImp& _view;
    std::vector<std::string> _text;
    ECFormat * _format;
};

// document controller

class ECDocumentController
{
public:
    ECDocumentController(ECDocument &document, ECCursor& cursor, ECCommandHistory& history, ECFormat& format);
    ~ECDocumentController();
    void InsertText(int keypress);                          // insert typed ASCII valued chars
    void RemoveText();                                      // determines backspace operation depending on if raw x = 0
    void Enter();                                           // picks which new line operation, this gets called by handler
    void NewLineNoChars(int y);                             // new line on end of line
    void NewLineChars(string chars, int x, int y);          // new line bringing text down
    bool Undo();                                            // ctrl-z
    bool Redo();                                            // ctrl-y
private:
    ECDocument& _document;
    ECCursor& _cursor;
    ECCommandHistory _history;
    ECFormat& _form;
};

#endif