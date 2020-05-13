#include "StringViewer.hpp"
#include "String.hpp"
#include "../Debugger/Debugger.hpp"

const String STRINGVIEWMODE = "STRING VIEW MODE";

const String PAGE = "Page ";

const String COMMAND_WINDOW = "Command: ";

const String HELP_MESSAGE = "This is the string view mode. It's used to easily view big strings.\n"
"It splits the string to pages of a given size.\n"
"You can navigate between pages using the commands:\n\"nextpage\", that goes to the next page,\n"
"\"prevpage\", that goes to the previous page,\n\"goto <page number>\" that goes to a specific page\n"
"and you can exit the string view mode with the \"exit\" command. ";

const String NOTVALIDCOMMAND_MESSAGE = "Not a valid command. Type \"help\" if you don't know what to do, "
"or \"exit\" to exit the string view mode.";

const String NEXTPAGE_COMMAND = "nextpage";
const String PREVPAGE_COMMAND = "prevpage";
const String GOTO_COMMAND = "goto";
const String HELP_COMMAND = "help";
const String EXIT_COMMAND = "exit";

const String TESTSTRING = "alabala\nhmm\ni ko sq\nnqkvi redove tuka\noshte edin\nyea\nnz ko sq\n"
"mii aide oshte\nda ima tam\nda testvam\noshte redove\ndadada\npickle riiiick\nfrench\n"
"hiphop\naide de\nkolko oshte\nmii oshte tolkova\nmaleee\nmn ue\nne e nujno da sa tolko dulgi ama da vidim\n"
"oshteee\nalabala\noctopus\nmi e lubimiq\nalbum\nno lubimata mi\npesen\nne e ot nego\n"
"a e lateralus\nna tool\nbasi qkata pesen\naide stiga";

StringViewer::StringViewer(const String& string, const String& pageBeginning, unsigned linesPerPage)
{
    this->string = string;
    this->pageBeginning = pageBeginning;
    this->linesPerPage = linesPerPage;

    //Split the string to lines
    this->lines = string.Split('\n');

    //Find the max page width
    this->maxPageWidth = 0;
    for (int i = 0; i < this->lines.GetLength(); i++)
    {
        unsigned currLineWidth = this->lines[i].GetLength();
        if (maxPageWidth < currLineWidth)
        {
            maxPageWidth = currLineWidth;
        }
    }

    //Find the dashes for string formatting
    this->dashesForStringViewer = String('-', (this->maxPageWidth - STRINGVIEWMODE.GetLength()) / 2);
    this->dashesForPage = String('-', (this->maxPageWidth - PAGE.GetLength()) / 2);
    this->dashesForPageEnd = String('-', this->maxPageWidth);

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
        std::cout << COMMAND_WINDOW;
        //Read the next command
        std::cin >> command;
    }
    //Execute command and stop if the command is "exit"
    while (this->ExecuteCommand(command));

    system("cls");
}

bool StringViewer::ExecuteCommand(const String& command)
{
    //Handle each command type separately
    if (command == NEXTPAGE_COMMAND)
    {
        this->NextPage();
    }
    else if (command == PREVPAGE_COMMAND)
    {
        this->PrevPage();
    }
    else if (command.StartsWith(GOTO_COMMAND) && command.Split().GetLength() == 2)
    {
        //Get the page number, the second word of the command
        Vector<String> args = command.Split();
        String pageNumberString = args[1];
        unsigned pageNumber = pageNumberString.ParseToInt();
        //Execute command
        this->GoTo(pageNumber - 1);
    }
    else if (command == HELP_COMMAND)
    {
        this->Help();
    }
    else if (command == EXIT_COMMAND)
    {
        return false;
    }
    else
    {
        this->currentScreen = this->GetPageWindow(this->currentPage) + "\n" +
        NOTVALIDCOMMAND_MESSAGE + "\n";
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
        "Error: Not a valid page number. Pages are from 1 to " + String::ParseFromInt(this->pagesCount) + ".\n";
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
    + HELP_MESSAGE + "\n\n";
}

String StringViewer::GetPageWindow(unsigned pageIndex)
{
    String page = this->GetPage(pageIndex);

    return this->dashesForStringViewer + STRINGVIEWMODE + this->dashesForStringViewer + "\n\n" + 
    this->dashesForPage + PAGE + String::ParseFromInt(pageIndex + 1) + this->dashesForPage + "\n\n" +
    page + "\n" +
    this->dashesForPageEnd + "\n";
}

String StringViewer::GetPage(unsigned pageIndex)
{
    String page = this->pageBeginning + "\n";
    //Find the indecies of the first and last line on the page
    unsigned firstLineIndex = pageIndex * this->linesPerPage;
    unsigned lastLineIndex = (pageIndex + 1) * this->linesPerPage - 1;
    //Add lines one by one
    for (int i = firstLineIndex; i <= lastLineIndex; i++)
    {
        if (i < this->lines.GetLength())
        {
            page += this->lines[i] + "\n";
        }
        else
        {
            page += "\n";
        }
    }

    return page;
}