#ifndef ECCOMMANDS_H
#define ECCOMMANDS_H

#include "ECCursor.h"
#include <vector>
#include <string>
class ECCursor;
class ECDocument;
class ECFormat;
using namespace std;

// command and history

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void Unexecute() = 0;
};

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    void ExecuteCommand(ECCommand * pCmd);
private:
    std::vector<ECCommand *> _commands;
    int _pos;
};


// cursor commands 


class TypeMove : public ECCommand           
{
public:
    TypeMove(int cx, int cy, ECCursor& cursor);
    ~TypeMove();
    void Execute();
    void Unexecute();
private:
    int _x;
    int _y;
    ECCursor& _cursor;
};

class BackSpaceMove : public ECCommand
{
public:
    BackSpaceMove(ECCursor& cursor);
    ~BackSpaceMove();
    void Execute();
    void Unexecute();
private:
    ECCursor& _cursor;
    int _x;
    int _y;
};

class NewLineCursor : public ECCommand
{
public:
    NewLineCursor(int cx, ECCursor& cursor);                            
    ~NewLineCursor();
    void Execute();                                                     
    void Unexecute();                                                   
private:
    int _x;                                                             
    ECCursor& _cursor;
    int _y;
};

class CursorUp : public ECCommand
{
public:
    CursorUp(int cx, ECCursor& cursor);                                 
    ~CursorUp();
    void Execute();                                                     
    void Unexecute();                                                   
private:
    int _x;
    ECCursor& _cursor;
    int _y;
};


// document commands 


class InsertText : public ECCommand
{
public:
    InsertText(int cx, int cy, int key, ECDocument& document);
    ~InsertText();
    void Execute();                                                 
    void Unexecute();                                               
private:
    int _x;                                                          
    int _y;                                                        
    int _key;                                                       
    ECDocument& _doc;                                              
};

class RemoveText : public ECCommand
{
public:
    RemoveText(int cx, int cy, ECDocument& document);
    ~RemoveText();
    void Execute();                                                 
    void Unexecute();                                               
private:
    int _x;                                                          
    int _y;
    ECDocument& _doc;
    char _key;                                                      
};

class LineRemoval : public ECCommand
{
public:
    LineRemoval(int cy, ECDocument& document);
    ~LineRemoval();
    void Execute();
    void Unexecute();
private:
    int _y;
    ECDocument& _doc;
};

class BackSpaceFrontChar : public ECCommand
{
public:
    BackSpaceFrontChar(int cx, int cy, string chars, ECDocument& document, ECFormat& format);
    ~BackSpaceFrontChar();
    void Execute();                                                 
    void Unexecute();                                               
private:
    int _x;                                                         
    int _y;
    string _chars; 
    ECDocument& _doc;
    ECFormat& _form;
    int _placement;                                                 
};

class NewLineNoChars : public ECCommand
{
public:
    NewLineNoChars(int cy, ECDocument& document);
    ~NewLineNoChars();
    void Execute();                                                 
    void Unexecute();                                               
private:
    int _y;                                                         
    ECDocument& _doc;
};

class NewLineChars : public ECCommand
{
public:
    NewLineChars(int cx, int cy, string chars, ECDocument& document, ECFormat& format);
    ~NewLineChars();
    void Execute();                                                 
    void Unexecute();                                               
private:
    int _x;                                                         
    int _y;
    string _chars;
    ECDocument& _doc;
    ECFormat& _form;
};

#endif