#include "ECDocument.h"
#include "ECFormat.h"
#include <algorithm>
using namespace std;

// document controller implementation

ECDocumentController::ECDocumentController(ECDocument &document, ECCursor& cursor, ECCommandHistory& history, ECFormat& format) : 
    _document(document), _cursor(cursor), _history(history), _form(format) {}

ECDocumentController::~ECDocumentController() {}

void ECDocumentController::InsertText(int keypress)
{
    int cursorx = _cursor.GetCursorx();
    int cursory = _cursor.GetCursory();
    ECCommand * cmd = new ::InsertText(cursorx, cursory, keypress, _document);
    _history.ExecuteCommand(cmd);
}

void ECDocumentController::RemoveText()
{
    int cursorx = _cursor.GetCursorx();
    int cursory = _cursor.GetCursory();
    if ((cursorx != 0) || (cursory != 0)) {
        if ((cursorx == 0) && (_document.WindowValidMove(cursorx, cursory-1))) {
            _cursor.SetPlacement(_document.GetRowSize(cursory-1));
            if (_document.EmptyLine(cursorx, cursory)) {
                ECCommand * cmd = new ::LineRemoval(cursory, _document);
                _history.ExecuteCommand(cmd);
            }
            else {
                string chars = _document.GetStringAt(cursorx, cursory);
                ECCommand * cmd = new ::BackSpaceFrontChar(cursorx, cursory, chars, _document, _form);
                _history.ExecuteCommand(cmd);
            }
        }
        else {
            ECCommand * cmd = new ::RemoveText(cursorx, cursory, _document);
            _history.ExecuteCommand(cmd);
        }
    }
}

void ECDocumentController::Enter()
{
    int cursorx = _cursor.GetCursorx();
    int cursory = _cursor.GetCursory();
    if (_document.GetCharAt(cursorx, cursory) != '\0') {
        string chars = _document.GetStringAt(cursorx, cursory);
        NewLineChars(chars, cursorx, cursory);
    }
    else if (_document.GetCharAt(cursorx, cursory) == '\0') {
        NewLineNoChars(cursory);
    }
}

void ECDocumentController::NewLineNoChars(int y)
{
    ECCommand * cmd = new ::NewLineNoChars(y, _document);
    _history.ExecuteCommand(cmd);
}

void ECDocumentController::NewLineChars(string chars, int x, int y) 
{
    ECCommand * cmd = new ::NewLineChars(x, y, chars, _document, _form);
    _history.ExecuteCommand(cmd);
}

bool ECDocumentController::Undo()
{
    if (_history.Undo()) {
        return true;
    }
    else {
        return false;
    }
}

bool ECDocumentController::Redo()
{
    if (_history.Redo()) {
        return true;
    }
    else {
        return false;
    }
}


// document implementation


ECDocument::ECDocument(ECTextViewImp& view, vector<string> textBuffer) : _view(view)
{
    for (auto line : textBuffer) {
        _text.push_back(line);
    }
}

ECDocument::~ECDocument() {}

char ECDocument::GetCharAt(int x, int y) { 
    int size = GetRowSize(y);
    if (x >= size) {
        return '\0';
    }
    else {
        return _text[y][x];
    }
}

string ECDocument::GetStringAt(int x, int y)
{
    string s(_text.at(y).begin() + x, _text.at(y).end());
    return s;
}

int ECDocument::GetRowSize(int y) {return _text[y].size();}

int ECDocument::GetNumRows() { return _text.size();}

int ECDocument::GetMaxRowLength()
{
    int cur_max = -1;
    int size;
    for (string s : _text) {
        size = s.size();
        if (size > cur_max)
            cur_max = size;
    }
    return cur_max;
}

void ECDocument::InsertCharAt(int x, int y, char letter)
{
    auto i = _text[y].begin() + x;
    _text[y].insert(i, letter);
    Send(y);
}

void ECDocument::RemoveCharAt(int x, int y)
{
    auto i = _text[y].begin() + x;
    _text[y].erase(i);
    Send(y);
}

void ECDocument::InsertStringAt(int x, int y, string text) {_text.at(y).insert(x, text);}

void ECDocument::RemoveStringAt(int x, int y) {_text.at(y).erase(_text.at(y).begin() + x, _text.at(y).end());}

void ECDocument::Newline(int y)
{
    auto i = _text.begin() + y + 1;
    _text.insert(i, string(""));
    _format->AddParagraph(y);
}

void ECDocument::RemoveLine(int y)
{
    auto i = _text.begin() + y;
    _text.erase(i);
    _format->RemoveParagraph(y);
}

string ECDocument::GetWordAtEnd(int x, int y)
{
    string s;
    for (auto i = _text[y].begin() + x; i != _text[y].begin(); i--) {
        if (isspace(*i)) {
            break;
        }
        else {
            s += *i;
        }
    }
    reverse(s.begin(), s.end());
    return s;
}   

bool ECDocument::DocValidMove(int x, int y) 
{
    if ((0 <= x) && (x <= GetRowSize(y)) && (0 <= y) && (y < GetNumRows())) {
        return true;
    }
    else {
        return false;
    }
}

bool ECDocument::CursorValidMove(int x, int y)
{
    if ((x >= 0) && x <= GetMaxRowLength() && (0 <= y) && (y < GetNumRows())) {
        return true;
    }
    else {
        return false;
    }
}

bool ECDocument::WindowValidMove(int x, int y)
{
    if ((x >= 0) && (x < _view.GetColNumInView()) && (y >= 0) && (y < _view.GetRowNumInView())) {
        return true;
    }
    else {
        return false;
    }
}

bool ECDocument::EmptyLine(int x, int y)
{
    for (auto i = _text[y].begin() + x; i != _text[y].end(); i++) {
        if (!isspace(*i))
            return false;
    }
    return true;   
}

void ECDocument::SetFormatter(ECFormat& format) {_format = &format;}

void ECDocument::Send(int y) {_format->Repair(y);}

vector<string> ECDocument::GetText()
{
    vector<string> rawText;
    for(auto para : _text) {
        rawText.push_back(para);
    }
    return rawText;
}
