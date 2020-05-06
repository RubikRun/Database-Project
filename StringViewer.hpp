#pragma once

#include "String.hpp"
#include "Vector.hpp"
#include "Constants.hpp"

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
        StringViewer(const String& string, unsigned linesPerPage);

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

StringViewer::StringViewer(const String& string, unsigned linesPerPage)
{
    this->string = string;
    this->linesPerPage = linesPerPage;

    //Split the string to lines
    this->lines = string.Split('\n');
    //We begin the view mode on page 1
    this->currentPage = 0;
    //Calculate the number of pages needed
    this->pagesCount = this->lines.GetLength() / this->linesPerPage +
    ( (this->lines.GetLength() % this->linesPerPage == 0) ? 0 : 1 );

    //Initially we are on the first page
    this->currentScreen = this->GetPageWindow(0) + "\n";
}

void StringViewer::ViewMode()
{
    String command;
    do
    {
        //Clear the screen
        system("cls");
        //Show the current screen + command window
        std::cout << this->currentScreen;
        std::cout << STRINGVIEWMODE_COMMANDWINDOW;
        //Read the next command
        std::cin >> command;
    }
    //Execute command and stop if the command is "exit"
    while (this->ExecuteCommand(command));
}

bool StringViewer::ExecuteCommand(const String& command)
{
    //Handle each command type separately
    if (command == STRINGVIEWMODE_NEXTPAGE_COMMAND)
    {
        this->NextPage();
    }
    else if (command == STRINGVIEWMODE_PREVPAGE_COMMAND)
    {
        this->PrevPage();
    }
    else if (command.StartsWith(STRINGVIEWMODE_GOTO_COMMAND))
    {
        //Get the page number, the second word of the command
        Vector<String> args = command.Split();
        String pageNumberString = args[1];
        unsigned pageNumber = pageNumberString.ParseToInt();
        //Execute command
        this->GoTo(pageNumber - 1);
    }
    else if (command == STRINGVIEWMODE_HELP_COMMAND)
    {
        this->Help();
    }
    else if (command == STRINGVIEWMODE_EXIT_COMMAND)
    {
        return false;
    }
    else
    {
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n" +
        STRINGVIEWMODE_NOTVALIDCOMMAND_MESSAGE + "\n";
    }

    return true;
}

void StringViewer::NextPage()
{
    //If we are on the last page, there is no next page
    if (this->currentPage + 1 >= this->pagesCount)
    {
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n"
        "Error: You are on the last page, there is no next page\n";
    }
    else
    {
        this->currentPage++;
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n";
    }
}

void StringViewer::PrevPage()
{
    //If we are on the first page, there is no previous page
    if (this->currentPage == 0)
    {
        this->currentScreen = this->GetPageWindow(0) + "\n"
        "Error: You are on the first page, there is no previous page\n";
    }
    else
    {
        this->currentPage--;
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n";
    }
}

void StringViewer::GoTo(unsigned requestPage)
{
    //If the requested page is beyond our pages, we cannot show it
    if (requestPage >= this->pagesCount)
    {
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n"
        "Error: Not a valid page number. Pages are from 1 to " + ParseFromInt(this->pagesCount) + ".\n";
    }
    else
    {
        this->currentPage = requestPage;
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n";
    }
}

void StringViewer::Help()
{
    this->currentScreen = this->GetPageWindow(this->currentPage) + "\n"
    + STRINGVIEWMODE_HELP_MESSAGE + "\n\n";
}

String StringViewer::GetPageWindow(unsigned pageIndex)
{
    String page = this->GetPage(pageIndex);

    return STRINGVIEWMODE_DASHES + STRINGVIEWMODE + STRINGVIEWMODE_DASHES + "\n" + 
    STRINGVIEWMODE_DASHES + STRINGVIEWMODE_PAGE + ParseFromInt(pageIndex + 1) + STRINGVIEWMODE_DASHES + "\n" +
    page + 
    STRINGVIEWMODE_DASHES + STRINGVIEWMODE_DASHES + STRINGVIEWMODE_DASHES + "\n";
}

String StringViewer::GetPage(unsigned pageIndex)
{
    String page = "";
    //Find the indecies of the first and last line on the page
    unsigned firstPageIndex = pageIndex * this->linesPerPage;
    unsigned lastPageIndex = (pageIndex + 1) * this->linesPerPage - 1;
    //Add lines one by one
    for (int i = firstPageIndex; i <= lastPageIndex; i++)
    {
        page += this->lines[i] + "\n";
    }

    return page;
}