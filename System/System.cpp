#include "System.hpp"
#include "../String/String.hpp"

#include <iostream>

static const String HELP_COMMAND = "help";
static const String OPEN_COMMAND = "open";
static const String SAVE_COMMAND = "save";
static const String AS = "as";
static const String CLOSE_COMMAND = "close";
static const String EXIT_COMMAND = "exit";

static const String INVALIDCOMMAND_MESSAGE =
"Not a valid command or there is no database loaded.\n"
"Type \"help\" if you are not sure what to do.\n";

static const String HELP_MESSAGE =
"Welcome to my database system!\n"
"It is used to manage databases, consisting of series of tables.\n"
"All databases and tables are stored in separate files.\n\n"
"You can open, close and save databases with the following commands:\n"
"   *open <filename> - to load a database from a file,\n"
"   *close - to close the currently loaded database,\n"
"   *save - to save the currently loaded database to its file,\n"
"   *save as <filename> - to save the currently loaded database to another file,\n"
"   *exit - to exit the database system,\n"
"   *help - to bring up this help message\n\n"
"Every table in a database consists of rows and columns and each column can be of different type.\n"
"Available column types are:\n"
"   *Whole, which stores a whole number,\n"
"   *Decimal, which stores a decimal number,\n"
"   *Text, which stores text,\n\n"
"When you have a database loaded, you can manage it with the following commands:\n"
"   *import <filename> - imports a table from a file to the database,\n"
"   *showtables - shows the names of all tables in the database,\n"
"   *describe <table name> - shows the types of all columns of a table,\n"
"   *print <table name> - shows all the rows of a table,\n"
"   *export <table name> <filename> - saves a table from the database to a file,\n"
"   *select <column index> <value> <table name> - shows the rows of a table\n"
"       that have the specified value in the specified column,\n"
"   *addcolumn <table name> <column name> <column type> - adds a new column to a table.\n"
"       The new column has the specified name and type and each row has a NULL value in the new column,\n"
"   *update <table name> <search column index> <search value> <target column index> <target value> -\n"
"       finds all the rows in a table that have the search value in the search column\n"
"       and updates their target column to have the target value,\n"
"   *delete <table name> <search column index> <search value> - finds the rows of a table\n"
"       that have the search value in the search column and deletes them,\n"
"   *insert <table name> <column 1 value> <column 2 value> ... <column n value> -\n"
"       adds a new row with the given values to a table,\n"
"   *innerjoin <table 1 name> <column index 1> <table 2 name> <column index 2> -\n"
"       inner joins the two tables on the given columns. Saves the result as a new table in the database\n"
"   *rename <old name> <new name> - changes the name of a table,\n"
"   *count <table name> <search column index> <search value> - counts the number of rows in a table\n"
"       that have the search value in the search column,\n"
"   *aggregate <table name> <search column index> <search value> <target column> <operation> -\n"
"       finds the rows from a table that have the search value in the search column\n"
"       and applies the operation to their target column.\n"
"       Available operations are: sum, product, maximum, minimum.\n";

System::System()
{
    this->database = nullptr;
}

void System::Run()
{
    String command;
    while (true)
    {
        std::cin >> command;
        if (command == EXIT_COMMAND)
        {
            break;
        }
        this->ExecuteCommand(command);
    }
}

void System::ExecuteCommand(const String& command)
{
    Vector<String> args = command.Split();

    if (args[0] == OPEN_COMMAND)
    {
        this->OpenCommand(args);
    }
    else if (command == SAVE_COMMAND)
    {
        this->SaveCommand();
    }
    else if (args[0] == SAVE_COMMAND && args.GetLength() == 3 && args[1] == AS)
    {
        this->SaveAsCommand(args);
    }
    else if (command == HELP_COMMAND)
    {
        this->HelpCommand();
    }
    else if (command == CLOSE_COMMAND)
    {
        if (this->database == nullptr)
        {
            return;
        }
        std::cout << "Database \"" << this->database->GetName() << "\" closed.\n" << std::endl;
        delete this->database;
        this->database = nullptr;
    }
    else if (this->database != nullptr)
    {
        this->database->ExecuteCommand(args);
    }
    else
    {
        std::cout << INVALIDCOMMAND_MESSAGE << std::endl;
    }
}

void System::OpenCommand(const Vector<String>& args)
{
    if (args.GetLength() < 2)
    {
        return;
    }

    if (this->database != nullptr)
    {
        std::cerr << "Error: There is already a database loaded.\n"
        "Close it before trying to load another one.\n" << std::endl;
        return;
    }

    //Extract filename
    String filename = args[1];
    //Load database
    this->database = Database::ReadDatabaseFromFile(filename);
    std::cout << "Database \"" << this->database->GetName() << "\" loaded.\n" << std::endl;
}

void System::SaveCommand()
{
    if (this->database == nullptr)
    {
        return;
    }
    this->database->WriteDatabaseToFile(this->database->GetFilename());
    std::cout << "Database \"" << this->database->GetName() << "\" saved successfully to its file: "
    << this->database->GetFilename() << std::endl;
}

void System::SaveAsCommand(const Vector<String>& args)
{
    if (this->database == nullptr)
    {
        return;
    }
    //Extract filename
    String filename = args[2];
    //Save database to file
    this->database->WriteDatabaseToFile(filename);
    std::cout << "Database \"" << this->database->GetName() << "\" saved successfully to file: "
    << filename << std::endl;
}

void System::HelpCommand()
{
    std::cout << HELP_MESSAGE << std::endl;
}
