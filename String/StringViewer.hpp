#pragma once

#include "String.hpp"
#include "../Vector/Vector.hpp"

//A class used for viewing a long string in view mode, page by page
//The user can navigate between pages using commands
class StringViewer
{
    //The string we want to view
    String string;
    //The string's lines separated
    Vector<String> lines;

    //Number of lines we want per page
    unsigned linesPerPage;
    //The index of the current page
    unsigned currentPage;
    //The total number of pages needed
    unsigned pagesCount;

    //The current screen
    String currentScreen;

    public:

        //Creates a string viewer for the given string with the specified number of lines per page
        StringViewer(const String& string, unsigned linesPerPage = 10);

        //Enters view mode
        void ViewMode();

    private:

        //Executes the given command, returns true if it wasn't "exit"
        bool ExecuteCommand(const String& command);

        //Executes the "nextpage" command
        void NextPage();

        //Executes the "prevpage" command
        void PrevPage();

        //Executes the "goto" command
        void GoTo(unsigned requestPage);

        //Executes the "help" command
        void Help();

        //Returns the whole window with the requested page
        String GetPageWindow(unsigned requestPage);

        //Returns the requested page as a string
        String GetPage(unsigned requestPage);
};