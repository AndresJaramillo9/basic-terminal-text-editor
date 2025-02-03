#include "ECTextViewImp.h"
#include "ECObserver.h"
#include "ECConcreteObserver.h"
#include "ECCursor.h"
#include "ECDocument.h"
#include "ECHandlers.h"
#include "ECFormat.h"
#include "ECStrategy.h"
#include <iostream>
#include <fstream>

using namespace  std;

int main(int argc, char *argv[])
{
    //std::cerr << "typing log: \n";
    
    fstream file;                                           // file IO
    vector<string> fileBuffer;
    if (argc == 2) {
        file.open(argv[1]);
        if (file.is_open()) {
            string line;
            while(getline(file, line)) {
                fileBuffer.push_back(line);
            }
            file.close();
        }
        else  {
            fileBuffer.push_back("");
        }
        file.open(argv[1], ios::trunc | ios::out);
        
        
        printf("\033[?1049h\033[H");                        // terminal code to avoid problems with quitting window
    
        ECTextViewImp wndTest;                              
    
        ECCommandHistory hist;
        ECCursor curs;
        ECDocument doc(wndTest, fileBuffer);
        
        ECFormat form(doc, curs, wndTest);
        doc.SetFormatter(form);
        curs.SetFormatter(form);                  
        
        ECCursorController cursCtrl(doc, curs, hist, form);
        ECDocumentController ctrl(doc, curs, hist, form);
        
        Typing TypeHandler(cursCtrl, ctrl, form);        
        Entering EnterHandler(cursCtrl, ctrl, form);
        Deleting DeleteHandler(cursCtrl, ctrl, form);
        Tracking TrackHandler(cursCtrl, ctrl, form);
        ArrowKeys ArrowHandler(cursCtrl, form);
        Observer observer(wndTest, TypeHandler);
        wndTest.Attach(&observer);
        TypeHandler.SetNext(&EnterHandler);
        EnterHandler.SetNext(&DeleteHandler);
        DeleteHandler.SetNext(&TrackHandler);
        TrackHandler.SetNext(&ArrowHandler);

        wndTest.AddStatusRow("Status", "Row", false);  
        
        form.Init();

        wndTest.Show();

        printf("\033[?1049l");                              // terminal code to avoid problems with quitting window

        vector<string> outfileBuffer = doc.GetText();       // file IO  
        for (auto line : outfileBuffer) {
            file << line << endl;
        }
        file.close();
    }
    else {
        cout << "Need two arguments: ./strops [filename.txt]" << endl;
    }
    return 0;
}
