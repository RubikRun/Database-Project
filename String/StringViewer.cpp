#include "StringViewer.hpp"
#include "String.hpp"

const String DASHES = "----------------";

const String STRINGVIEWMODE = "STRING VIEW MODE";

const String PAGE = "Page ";

const String COMMANDWINDOW = "Command: ";

const String HELP_MESSAGE = "This is the string view mode. It's used to easily view big strings. "
"It splits the string to pages, with no more than n lines per page. "
"You can navigate between pages using the commands \"nextpage\", that goes to the next page, "
"\"prevpage\", that goes to the previous page, \"goto <page number>\" that goes to a specific page "
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
        std::cout << COMMANDWINDOW;
        //Read the next command
        std::cin >> command;
    }
    //Execute command and stop if the command is "exit"
    while (this->ExecuteCommand(command));
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
    + HELP_MESSAGE + "\n\n";
}

String StringViewer::GetPageWindow(unsigned pageIndex)
{
    String page = this->GetPage(pageIndex);

    return DASHES + STRINGVIEWMODE + DASHES + "\n" + 
    DASHES + PAGE + ParseFromInt(pageIndex + 1) + DASHES + "\n" +
    page + 
    DASHES + DASHES + DASHES + "\n";
}

String StringViewer::GetPage(unsigned pageIndex)
{
    String page = "";
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