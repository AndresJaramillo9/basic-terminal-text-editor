#ifndef ECFORMAT_H
#define ECFORMAT_H

#include <vector>
#include <string>
#include "ECTextViewImp.h"
#include "ECStrategy.h"
class ECDocument;
class ECCursor;
using namespace std;


class ECParagraph
{
public:
    ECParagraph(string text = "");                                          
    ~ECParagraph() {}           
    vector<string> GetParagraph();                                          // return the formatted paragraph paragraph 
    int NumRows();                                                          // get number of lines in paragraph
    int RowSize(int y);                                                     // get size of line at row y of paragraph
    void UpdateRows(vector<string> lines);                                  // recieves new formatted lines from formatter calling to strategy and updates paragraph
private:
    vector<string> _rows;
};

class ECFormat
{
public:
    ECFormat(ECDocument& document, ECCursor& cursor, ECTextViewImp& view);  
    ~ECFormat();                                                            // deletes paragraphs from heap
    void Init();                                                            // reads in lines from raw text, makes and formats paragraphs 
    void SetPageNumber(int y);                                              // establishes page number given lines cursor (x,y)
    void SetParagraphNumber(int y);                                         // establishes the current paragraph index
    int GetPageNumber();                                                    // returns 0-based page number 
    int GetParagraphNumber();                                               // returns 0-based paragraph number 
    int GetRowSize(int y);                                                  // get size of line at row y of formatted text
    int GetNumRows();                                                       // get number of lines of formatted text
    int GetMaxRowLength();                                                  // get size of line with maximum size, used for smartter cursor movements
    int GetLinesCursorx();                                                  // get line cursor x position
    int GetLinesCursory();                                                  // get line cursor y position
    bool DocValidMove(int x, int y);                                        // used for cursor movements left and right, relative to line cursor
    bool CursorValidMove(int x, int y);                                     // used for cursor movement up and down, relative to line cursor
    bool WindowValidMove(int x, int y);                                     // used for text and cursor movemnt in reference to window, relative to line cursor
    void AddParagraph(int y, string text = "");                             // insert paragraph at yth index of paragraph vector
    void RemoveParagraph(int y);                                            // removes yth index from paragraph vector
    ECParagraph * GetParagraph(int y);                                      // get the yth paragraph in vector
    void ArrowKeyDisplay(char direction);                                   // controls the arrow key movements on the display, updates the all objects' internals accordingly
    void CurRawCursor(int x, int y);                                        // gets raw cursor (x,y) and maps into lines cursor (x,y) and sends to update
    void UpdateDisplayCursor(int x, int y);                                 // update display cursor
    void UpdateLineCursor(int x, int y);                                    // update line cursor
    void UpdateRawCursor(int cx, int cy);                                   // establishes mapping of line cursor (x,y) to raw cursor (x,y) and sends to raw cursor 
    void UpdateLines();                                                     // updates formatted lines
    void Repair(int y);                                                     // calls on strategy object to line break the yth paragraph and update that paragraph 
    void DocDisplay();                                                      // uses page number to display the right set of lines and sends info to doc send
    void CursorDisplay();                                                   // maps the lines cursor (x,y) to the display cursor (x,y) and sends to cursor send
    void DocSend(vector<string> displayText);                               // sends to view 
    void CursorSend();                                                      // sends to view

private:
    ECDocument& _doc;
    ECCursor& _curs;
    ECTextViewImp& _view;
    int _displaycursorx;                                                    // initally 0
    int _displaycursory;                                                    // initially 0
    int _linescursorx;                                                      // initially 0
    int _linescursory;                                                      // initially 0
    int _pageNum;                                                           // intially 0, 0-based
    int _curParagraph;                                                      // initially 0, 0-based
    vector<ECParagraph *> _paragraphs;
    vector<string> _formattedLines;
};

#endif