#include "Database.hpp"

#include <iostream>
#include <fstream>

static const String DATABASEFILE_PREFIX = "DataFiles/";
static const char DATABASEFILE_SEPARATOR = ',';

static const String OPERATION_IMPORT = "import";
static const String OPERATION_SHOWTABLES = "showtables";
static const String OPERATION_DESCRIBE = "describe";
static const String OPERATION_PRINT = "print";
static const String OPERATION_EXPORT = "export";
static const String OPERATION_SELECT = "select";
static const String OPERATION_ADDCOLUMN = "addcolumn";
static const String OPERATION_UPDATE = "update";
static const String OPERATION_DELETE = "delete";
static const String OPERATION_INSERT = "insert";
static const String OPERATION_INNERJOIN = "innerjoin";
static const String OPERATION_RENAME = "rename";
static const String OPERATION_COUNT = "count";
static const String OPERATION_AGGREGATE = "aggregate";

Database::Database()
{
    this->tablesCount = 0;
}

Database Database::ReadDatabaseFromFile(const String& filename)
{
    //Create an empty database
    Database database;
    database.filename = filename;

    //Open file
    std::ifstream file((DATABASEFILE_PREFIX + filename).GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Read database's name
    database.name.ReadNext(file, DATABASEFILE_SEPARATOR);
    //Read the number of tables
    database.tablesCount = String::ReadInt(file, DATABASEFILE_SEPARATOR);
    //Read tables
    for (int i = 0; i < database.tablesCount; i++)
    {
        //Read table's name
        String tableName;
        tableName.ReadNext(file, DATABASEFILE_SEPARATOR);
        //Read table's filename
        String tableFilename;
        tableFilename.ReadNext(file, DATABASEFILE_SEPARATOR);

        //Read the table from the file
        Table table = Table::ReadTableFromFile(tableFilename);
        //Add table to database
        database.tables.Add(table);
    }

    return database;
}

void Database::ExecuteCommand(const String& command)
{
    if (command.StartsWith(OPERATION_IMPORT))
    {
        this->ImportCommand(command);
    }
    else if (command.StartsWith(OPERATION_SHOWTABLES))
    {
        this->ShowtablesCommand();
    }
    else if (command.StartsWith(OPERATION_DESCRIBE))
    {

    }
    else if (command.StartsWith(OPERATION_PRINT))
    {

    }
    else if (command.StartsWith(OPERATION_EXPORT))
    {

    }
    else if (command.StartsWith(OPERATION_SELECT))
    {

    }
    else if (command.StartsWith(OPERATION_ADDCOLUMN))
    {

    }
    else if (command.StartsWith(OPERATION_UPDATE))
    {

    }
    else if (command.StartsWith(OPERATION_DELETE))
    {

    }
    else if (command.StartsWith(OPERATION_INSERT))
    {

    }
    else if (command.StartsWith(OPERATION_INNERJOIN))
    {

    }
    else if (command.StartsWith(OPERATION_RENAME))
    {

    }
    else if (command.StartsWith(OPERATION_COUNT))
    {

    }
    else if (command.StartsWith(OPERATION_AGGREGATE))
    {

    }
    else
    {

    }
}

void Database::ImportCommand(const String& command)
{
    //Split the command
    Vector<String> args = command.Split();
    if (args.GetLength() < 2)
    {
        return;
    }
    //Extract table's filename
    String tableFilename = args[1];
    //Load the table from the file
    Table table = Table::ReadTableFromFile(tableFilename);
    if (table.GetName().IsNull())
    {
        return;
    }
    //Check if name is unique
    bool found = false;
    for (int i = 0; i < this->tablesCount; i++)
    {
        if (this->tables[i].GetName() == table.GetName())
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        std::cerr << "Error: A table with name " << table.GetName() << " already exists." << std::endl;
        return;
    }
    //Add the table to database's tables
    this->tables.Add(table);
    this->tablesCount++;
}

void Database::ShowtablesCommand()
{
    std::cout << "All tables:" << std::endl;
    for (int i = 0; i < this->tablesCount; i++)
    {
        std::cout << i + 1 << ". " << this->tables[i].GetName() << std::endl;
    }
    std::cout << std::endl;
}