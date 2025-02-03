#ifndef ECSTRATEGY_H
#define ECSTRATEGY_H

#include <vector>
#include <string>
#include "ECTextViewImp.h"
class ECDocument;
class ECFormat;
using namespace std;

// Strategy 

class ECStrategyLineBreak
{
public:
    virtual ~ECStrategyLineBreak() {} 
    virtual vector<string> Repair()=0;
};

class ECStrategyArrowKey
{
public:
    virtual ~ECStrategyArrowKey() {}
    virtual void Repair()=0;
};

// line breaking algorithm 

class ECLineBreak : public ECStrategyLineBreak
{
public:
    ECLineBreak(ECDocument& document, ECTextViewImp& view, int y);
    ~ECLineBreak();
    vector<string> Repair();                     
private:
    ECDocument& _doc;
    ECTextViewImp& _view;
    int _paragraph;
};

// arrow key algorithm

class ECArrowKey : public ECStrategyArrowKey
{
public:
    ECArrowKey(ECFormat& format, char direction);
    ~ECArrowKey();
    void Repair();
private:
    ECFormat& _form;
    char _direction;
};

#endif