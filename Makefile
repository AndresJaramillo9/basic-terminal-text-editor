# borrows from one of the assignments

CPPFILES = ECCommands.cpp\
 ECCursor.cpp\
 ECDocument.cpp\
 ECEditorTest.cpp\
 ECFormat.cpp\
 ECHandlers.cpp\
 ECObserver.cpp\
 ECStrategy.cpp\
 ECTextViewImp.cpp\
 

HFILES = $(CPPFILES:.cpp=.h)

OFILES= $(CPPFILES:.cpp=.o)

CC = g++
LIBPATH =

CFLAGS = -g -std=c++14 -Wall -I.

all: stropsmain

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

stropsmain:$(OFILES)
	$(CC) -o strops $(OFILES)

clean: 
	rm *.o
	rm strops
