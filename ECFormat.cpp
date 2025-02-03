#include "ECFormat.h"
#include "ECStrategy.h"
#include "ECDocument.h"
#include <vector>
using namespace std;

// paragraph

ECParagraph::ECParagraph(string text) {_rows.push_back(text);}

vector<string> ECParagraph::GetParagraph() {return _rows;}

void ECParagraph::UpdateRows(vector<string> lines) { _rows = lines;}

int ECParagraph::NumRows() {return _rows.size();}

int ECParagraph::RowSize(int y) { return _rows[y].size();}


// format


ECFormat::ECFormat(ECDocument& document, ECCursor& cursor,ECTextViewImp& view) : 
    _doc(document), _curs(cursor), _view(view), _displaycursorx(0), _displaycursory(0), 
    _linescursorx(0), _linescursory(0), _pageNum(0), _curParagraph(0) {}

ECFormat::~ECFormat()
{
    for (auto para : _paragraphs) {
        delete para;
    }
}

void ECFormat::Init()
{
    vector<string> rawText = _doc.GetText();
    int count = 0;
    for (auto line: rawText) {
        ECParagraph * paragraph = new ECParagraph(line);
        _paragraphs.push_back(paragraph);
        count++;
    }
    for (int i=0; i < count; i++) {
        ECStrategyLineBreak * strat = new ECLineBreak(_doc, _view, i);
        vector<string> brokenLines = strat->Repair();
        _paragraphs[i]->UpdateRows(brokenLines);
        delete strat;
    }
    UpdateLines();
    DocDisplay();
    CursorDisplay();
}

void ECFormat::AddParagraph(int y, string text)
{
    ECParagraph * paragraph = new ECParagraph(text);
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y + 1;
    if (_paragraphs.size() == 0) {
        _paragraphs.push_back(paragraph);
    }
    else {
        _paragraphs.insert(i, paragraph);
    }
    UpdateLines();
}

void ECFormat::RemoveParagraph(int y)
{
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y;
    _paragraphs.erase(i);
    UpdateLines();
}

int ECFormat::GetPageNumber() {return _pageNum;}

int ECFormat::GetParagraphNumber() {return _curParagraph;}

int ECFormat::GetLinesCursorx() {return _linescursorx;}

int ECFormat::GetLinesCursory() {return _linescursory;}

int ECFormat::GetRowSize(int y) {return _formattedLines[y].size();}

int ECFormat::GetNumRows() { return _formattedLines.size();}

int ECFormat::GetMaxRowLength()
{
    int cur_max = -1;
    int size;
    for (string s : _formattedLines) {
        size = s.size();
        if (size > cur_max) {
            cur_max = size;
        }
    }
    return cur_max;
}

bool ECFormat::DocValidMove(int x, int y)
{
    if ((0 <= x) && (x <= GetRowSize(y)) && (0 <= y) && (y < GetNumRows())) {
        return true;
    }
    else {
        return false;
    }
}

bool ECFormat::CursorValidMove(int x, int y)
{
    if ((x >= 0) && (x <= GetMaxRowLength()) && (0 <= y) && (y < GetNumRows())) {
        return true;
    }
    else {
        return false;
    }
}

bool ECFormat::WindowValidMove(int x, int y)
{
    if ((x >= 0) && (x <= _view.GetColNumInView()) && (y >= 0) && (y < GetNumRows())) { // was x <
        return true;
    }
    else {
        return false;
    }
}

void ECFormat::SetPageNumber(int y)
{
    int pageRows = _view.GetRowNumInView();
    _pageNum = y / pageRows;
}

void ECFormat::SetParagraphNumber(int y)
{
    int paraRowCount;
    int rowCount = 0;
    int count = 0;
    for (auto para : _paragraphs) {
        paraRowCount = para->NumRows();
        rowCount += paraRowCount;
        if ((rowCount-1) >= y) {
            _curParagraph = count;
            break;
        }
        else {
            count++;
        }
    }
}

ECParagraph * ECFormat::GetParagraph(int y)
{
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y;
    return (*i);
}

void ECFormat::Repair(int y)
{
    _curParagraph = y;
    ECStrategyLineBreak * strat = new ECLineBreak(_doc, _view, _curParagraph);
    vector<string> lines = strat->Repair();
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y;
    (*i)->UpdateRows(lines);
    UpdateLines();
    delete strat;
}

void ECFormat::UpdateLines()
{
    vector<string> formattedText;
    for(auto paragraph : _paragraphs) {
        vector<string> p = paragraph->GetParagraph();
        for (auto row : p) {
            formattedText.push_back(row);
        }
    }
    _formattedLines = formattedText;
}

void ECFormat::ArrowKeyDisplay(char direction)
{
    ECStrategyArrowKey * strat = new ECArrowKey(*this, direction);
    strat->Repair();
    delete strat;
}

void ECFormat::UpdateRawCursor(int cx, int cy)          
{
    int y = GetParagraphNumber();
    int rows = 0;
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y;
    for(vector<ECParagraph*>::iterator j = _paragraphs.begin(); j != i; j++) {
        rows += (*j)->NumRows();
    }
    int difference = cy - rows ;
    int x = 0;
    for(int k= 0; k < difference; k++) {
        x += (*i)->RowSize(k);
    }
    x += cx;
    _curs.Map(x, y); 
}

void ECFormat::CurRawCursor(int x, int y)           
{
    int cx = 0;
    int cy = 0;
    vector<ECParagraph*>::iterator i = _paragraphs.begin() + y;
    for(vector<ECParagraph*>::iterator j = _paragraphs.begin(); j != i; j++) {
        cy += (*j)->NumRows();
    }
    int rowsize;
    int numrows = (*i)->NumRows();
    for (int k = 0; k < numrows; k++) {
        rowsize = (*i)->RowSize(k);
        if (x <= rowsize) {
            if (WindowValidMove(x, cy)) {
                cx = x;
            }
            else {
                cx = 0;
                cy++;
            }
            break;
        }
        else {
            cy++;
            x -= rowsize;
        }
    }
    UpdateLineCursor(cx, cy);
    SetPageNumber(cy);
}

void ECFormat::UpdateDisplayCursor(int x, int y)
{
    _displaycursorx = x;
    _displaycursory = y;
}

void ECFormat::UpdateLineCursor(int x, int y)
{
    _linescursorx = x;
    _linescursory = y;
}

void ECFormat::DocDisplay()
{
    int page = GetPageNumber();
    int startRow = _view.GetRowNumInView() * page;
    auto i = _formattedLines.begin() + startRow;
    vector<string>::iterator j;
    int endRow = startRow + _view.GetRowNumInView();
    if (GetNumRows() > endRow) {
        j = _formattedLines.begin() + endRow;
    }
    else {
        j = _formattedLines.begin() + GetNumRows();
    }
    vector<string> displayText(i, j);
    DocSend(displayText);
}

void ECFormat::CursorDisplay()
{
    int x, y;
    int page = GetPageNumber();
    int offsetY = GetLinesCursory() - (_view.GetRowNumInView() * page);
    y = offsetY;
    x = GetLinesCursorx();
    UpdateDisplayCursor(x,y);
    CursorSend();
}

void ECFormat::DocSend(vector<string> display)
{
    _view.InitRows();
    for (auto row : display) {          
        _view.AddRow(row);
    }
}

void ECFormat::CursorSend()
{
    _view.SetCursorX(_displaycursorx);
    _view.SetCursorY(_displaycursory);
}
