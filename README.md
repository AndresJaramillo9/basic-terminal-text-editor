## Usage
Compile
```
make
```
Run
```
./strops filename.txt
```
Clean
```
make clean
```

## About
The program starts by taking two arguments on the command line: ./strops [filename.txt]. The provided text file can be a new file in which the document displays a new blank document, or the text file can already exist in which the document will display the contents of the text file. Once the display is active, any alphanumeric and space keypress will be displayed at the location of the cursor. Pressing backspace will remove text or merge paragraphs depending on if the
cursor is at the front of a paragraph. Pressing enter will create a new paragraph. If the cursor is at end of the current paragraph, a new blank paragraph will be created. If the cursor is within the current paragraph, the paragraph gets broken up. Undoing and redoing text operations can be done with Ctrl-Z and Ctrl-Y, respectively. The arrows keys can be used to manually move the cursor within the bounds of the text. Moving right off the view will bring the cursor down a line if there is text available and moving left off the view will bring the cursor up a row if there is text available. To exit and save the document for later use under the filename, press Ctrl-Q.
The project was created using MacOS, so the compiling was conducted through Mac’s built-in clang compiler. As such, the Makefile successfully compiles the entire code when using a Mac. There exist some C-libraries that are not compatible between Mac and PC when using make, but the project does not use these dependencies, so using make should also work with PC. This is a relatively new fix, so if an error occurs in make, the following alternative method for compiling will work as well: c++ *.cpp -o strops -std=c++14.