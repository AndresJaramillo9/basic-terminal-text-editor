#include "ECStrategy.h"
#include "ECFormat.h"
#include "ECDocument.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// line break

ECLineBreak::ECLineBreak(ECDocument& document, ECTextViewImp& view, int y) : _doc(document), _view(view), _paragraph(y) {}

ECLineBreak::~ECLineBreak() {}

vector<string> ECLineBreak::Repair()
{
    vector<string> formatLines;
    formatLines.reserve(1);
    formatLines.push_back("");
    string rawText = _doc.GetStringAt(0, _paragraph);
    int length = _doc.GetRowSize(_paragraph);
    int Windowlength = _view.GetColNumInView();
    int x = 0;
    int y = 0;
    int size;
    for(int i=0; i < length; i++) {
        if (x < Windowlength) {
            formatLines[y].push_back(rawText[i]);
            x++;
        }
        else {
            if (_doc.GetCharAt(i-1, _paragraph) == ' ') {
                formatLines.push_back("");
                y++;
                x = 0;
                formatLines[y].push_back(rawText[i]);
                x++;
            }
            else {
                string word = _doc.GetWordAtEnd(i-1, _paragraph);
                size = word.size();
                for (int j=0; j < size; j++) {
                    formatLines[y].pop_back();
                } 
                formatLines.push_back("");
                y++;
                for (auto ch : word) {
                    formatLines[y].push_back(ch);
                }
                x = size;
                formatLines[y].push_back(rawText[i]);
                x++;
            }
        }
    }
    return formatLines;
}


// arrow key movement


ECArrowKey::ECArrowKey(ECFormat& format, char direction) : _form(format), _direction(direction) {}

ECArrowKey::~ECArrowKey() {}

void ECArrowKey::Repair() 
{
    int cursorx = _form.GetLinesCursorx();
    int cursory = _form.GetLinesCursory();
    int placement;
    if ((_direction == 'u') && _form.CursorValidMove(cursorx, cursory-1) && _form.WindowValidMove(cursorx, cursory-1)) {               
        int rowSizeAbove = _form.GetRowSize(cursory-1);
        if (rowSizeAbove >= cursorx) {
            placement = cursorx;
        }
        else {
            placement = rowSizeAbove;  
        }      
        _form.UpdateLineCursor(placement, cursory-1);
        _form.SetPageNumber(cursory-1);
        _form.SetParagraphNumber(cursory-1);
        _form.UpdateRawCursor(placement, cursory-1); 
    }
    else if ((_direction == 'd') && _form.CursorValidMove(cursorx, cursory+1) && _form.WindowValidMove(cursorx, cursory+1)) {
        int rowSizeBelow = _form.GetRowSize(cursory+1);
        if (rowSizeBelow >= cursorx) {
            placement = cursorx;
        }
        else {
            placement = rowSizeBelow;
        }
        _form.UpdateLineCursor(placement, cursory+1);
        _form.SetPageNumber(cursory+1);
        _form.SetParagraphNumber(cursory+1);
        _form.UpdateRawCursor(placement, cursory+1);
    }
    else if (_direction == 'l') {
        if (_form.DocValidMove(cursorx-1, cursory) && _form.WindowValidMove(cursorx-1, cursory)) {
            _form.UpdateLineCursor(cursorx-1, cursory);
            _form.SetPageNumber(cursory);
            _form.SetParagraphNumber(cursory);
            _form.UpdateRawCursor(cursorx-1, cursory);
        }
        else if ((cursorx == 0) && _form.CursorValidMove(cursorx, cursory-1) && _form.WindowValidMove(cursorx, cursory-1)) {
            int rowSizeAbove = _form.GetRowSize(cursory-1);
            _form.UpdateLineCursor(rowSizeAbove, cursory-1);
            _form.SetPageNumber(cursory-1);
            _form.SetParagraphNumber(cursory-1);
            _form.UpdateRawCursor(rowSizeAbove, cursory-1);
        }
    }
    else if (_direction == 'r') {
        if ((cursorx >= (_form.GetRowSize(cursory)) && _form.WindowValidMove(0, cursory+1))) { 
            _form.UpdateLineCursor(0, cursory+1);
            _form.SetPageNumber(cursory+1);
            _form.SetParagraphNumber(cursory+1);
            _form.UpdateRawCursor(0, cursory+1);
        }
        else if (_form.DocValidMove(cursorx+1, cursory) && _form.WindowValidMove(cursorx+1, cursory)) {
            _form.UpdateLineCursor(cursorx+1, cursory);
            _form.SetPageNumber(cursory);
            _form.SetParagraphNumber(cursory);
            _form.UpdateRawCursor(cursorx+1, cursory);
        }
    }
}
