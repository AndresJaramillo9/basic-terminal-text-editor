#ifndef ECHANDLERS_H
#define ECHANDLERS_H

class ECTextViewImp;
class ECDocument;
class ECDocumentController;
class ECCursor;
class ECCursorController;
class ECFormat;

// handlers

class Handler
{
public:
    virtual Handler * SetNext(Handler * handler) = 0;  
    virtual void Handle(int key) = 0;               
};

class AbstractHandler : public Handler
{
public:
    AbstractHandler();
    Handler * SetNext(Handler * handler);
    void Handle(int key);
protected:
    Handler * _next;
};

class Typing : public AbstractHandler
{
public:
    Typing(ECCursorController& cursor, ECDocumentController& document, ECFormat& form) : _cursorctrl(cursor), _docctrl(document), _form(form) {}
    ~Typing() {}
    void Handle(int key);
private:
    ECCursorController& _cursorctrl;
    ECDocumentController& _docctrl;
    ECFormat& _form;
};

class Entering : public AbstractHandler         
{
public:
    Entering(ECCursorController& cursor, ECDocumentController& document, ECFormat& form) :  _cursorctrl(cursor), _docctrl(document), _form(form) {}
    ~Entering() {}
    void Handle(int key);
private:
    ECCursorController& _cursorctrl;
    ECDocumentController& _docctrl; 
    ECFormat& _form;   
};

class Deleting : public AbstractHandler         
{
public:
    Deleting(ECCursorController& cursor, ECDocumentController& document, ECFormat& form) : _cursorctrl(cursor), _docctrl(document), _form(form) {}
    ~Deleting() {}
    void Handle(int key);
private:
    ECCursorController& _cursorctrl;
    ECDocumentController& _docctrl;
    ECFormat& _form;
};

class Tracking : public AbstractHandler         
{
public:
    Tracking(ECCursorController& cursor, ECDocumentController& document, ECFormat& form) : _cursorctrl(cursor), _docctrl(document), _form(form) {}
    ~Tracking() {}
    void Handle(int key);
private:
    ECCursorController& _cursorctrl;
    ECDocumentController& _docctrl;
    ECFormat& _form;
};

class ArrowKeys : public AbstractHandler
{
public:
    ArrowKeys(ECCursorController& cursor, ECFormat& form) : _cursorctrl(cursor), _form(form) {}
    ~ArrowKeys() {}
    void Handle(int key);
private:
    ECCursorController& _cursorctrl;
    ECFormat& _form;

};

#endif