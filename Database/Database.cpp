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

static const String INVALIDCOMMAND_MESSAGE =
"Not a valid command.\n"
"Type \"help\" if you are not sure what to do.\n";

Database::Database()
{
    this->tablesCount = 0;
}

const String& Database::GetName()
{
    return this->name;
}

const String& Database::GetFilename()
{
    return this->filename;
}

Database* Database::ReadDatabaseFromFile(const String& filename)
{
    //Open file
    std::ifstream file((DATABASEFILE_PREFIX + filename).GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return nullptr;
    }

    //Create an empty database
    Database* database = new Database();
    database->filename = filename;

    //Read database's name
    database->name.ReadNext(file, DATABASEFILE_SEPARATOR);
    //Read the number of tables
    database->tablesCount = String::ReadInt(file, DATABASEFILE_SEPARATOR);
    //Read tables
    for (int i = 0; i < database->tablesCount; i++)
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
        database->tables.Add(table);
    }

    //Close file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot clos file: " << filename << std::endl;
    }

    return database;
}

void Database::WriteDatabaseToFile(const String& filename)
{
    //Open file
    std::ofstream file((DATABASEFILE_PREFIX + filename).GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return;
    }

    //Write database's name
    file << this->name << DATABASEFILE_SEPARATOR;
    //Write the number of tables
    file << this->tablesCount << std::endl;
    //Write tables
    for (int i = 0; i < this->tablesCount; i++)
    {
        Table* table = &this->tables[i];
        //Write table's name and filename
        file << table->GetName() << DATABASEFILE_SEPARATOR
        << table->GetFilename() << std::endl;
    }

    //Close file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot clos file: " << filename << std::endl;
    }
}

void Database::ExecuteCommand(const Vector<String>& args)
{
    if (args[0] == OPERATION_IMPORT)
    {
        this->ImportCommand(args);
    }
    else if (args[0] == OPERATION_SHOWTABLES)
    {
        this->ShowtablesCommand();
    }
    else if (args[0] == OPERATION_DESCRIBE)
    {
        this->DescribeCommand(args);
    }
    else if (args[0] == OPERATION_PRINT)
    {
        this->PrintCommand(args);
    }
    else if (args[0] == OPERATION_EXPORT)
    {
        this->ExportCommand(args);
    }
    else if (args[0] == OPERATION_SELECT)
    {
        this->SelectCommand(args);
    }
    else if (args[0] == OPERATION_ADDCOLUMN)
    {
        this->AddcolumnCommand(args);
    }
    else if (args[0] == OPERATION_UPDATE)
    {
        this->UpdateCommand(args);
    }
    else if (args[0] == OPERATION_DELETE)
    {
        this->DeleteCommand(args);
    }
    else if (args[0] == OPERATION_INSERT)
    {
        this->InsertCommand(args);
    }
    else if (args[0] == OPERATION_INNERJOIN)
    {
        this->InnerjoinCommand(args);
    }
    else if (args[0] == OPERATION_RENAME)
    {
        this->RenameCommand(args);
    }
    else if (args[0] == OPERATION_COUNT)
    {
        this->CountCommand(args);
    }
    else if (args[0] == OPERATION_AGGREGATE)
    {
        this->AggregateCommand(args);
    }
    else
    {
        std::cout << INVALIDCOMMAND_MESSAGE << std::endl;
    }
}

void Database::ImportCommand(const Vector<String>& args)
{
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
    Table* sameNameTable = this->FindTable(table.GetName(), false);
    if (sameNameTable != nullptr)
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

void Database::DescribeCommand(const Vector<String>& args)
{
    if (args.GetLength() < 2)
    {
        return;
    }
    //Extract table's name
    String tableName = args[1];
    //Find table with that name
    Table* foundTable = this->FindTable(tableName);
    if (foundTable == nullptr)
    {
        return;
    }
    //Print table's columns' types
    foundTable->PrintColsInfo();
}

void Database::PrintCommand(const Vector<String>& args)
{
    if (args.GetLength() < 2)
    {
        return;
    }
    //Extract table's name
    String tableName = args[1];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Show all table's rows in view mode
    table->SelectAndView();
}

void Database::ExportCommand(const Vector<String>& args)
{
    if (args.GetLength() < 3)
    {
        return;
    }
    //Extract table's name and filename
    String tableName = args[1];
    String filename = args[2];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Write table to file
    table->WriteToFile(filename);
}

void Database::SelectCommand(const Vector<String>& args)
{
    if (args.GetLength() < 4)
    {
        return;
    }
    //Extract search column, search value and table's name
    unsigned searchColumn = args[1].ParseToInt();
    String searchValue = args[2];
    String tableName = args[3];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Show matching rows in view mode
    table->SelectAndView(searchColumn, searchValue);
}

void Database::AddcolumnCommand(const Vector<String>& args)
{
    if (args.GetLength() < 4)
    {
        return;
    }
    //Extract table's name, column's name and type
    String tableName = args[1];
    String colName = args[2];
    ValueType colType = Table::GetValueTypeFromString(args[3]);
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Add column in table
    table->AddColumn(colName, colType);
}

void Database::UpdateCommand(const Vector<String>& args)
{
    if (args.GetLength() < 6)
    {
        return;
    }
    //Extract all
    String tableName = args[1];
    unsigned searchCol = args[2].ParseToInt();
    String searchValue = args[3];
    unsigned targetCol = args[4].ParseToInt();
    String targetValue = args[5];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Update table's rows
    table->UpdateRows(searchCol, searchValue, targetCol, targetValue);
}

void Database::DeleteCommand(const Vector<String>& args)
{
    if (args.GetLength() < 4)
    {
        return;
    }
    //Extract all
    String tableName = args[1];
    unsigned searchCol = args[2].ParseToInt();
    String searchValue = args[3];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Delete matching rows
    table->DeleteRows(searchCol, searchValue);
}

void Database::InsertCommand(const Vector<String>& args)
{
    if (args.GetLength() < 2)
    {
        return;
    }
    //Extract table's name
    String tableName = args[1];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Extract row
    Row row = args;
    row.RemoveAt(0);
    row.RemoveAt(0);
    //Add row to table
    table->AddRow(row);
}

void Database::InnerjoinCommand(const Vector<String>& args)
{
    if (args.GetLength() < 5)
    {
        return;
    }
    //Extract tables' names and columns
    String table1Name = args[1];
    unsigned col1 = args[2].ParseToInt();
    String table2Name = args[3];
    unsigned col2 = args[4].ParseToInt();
    //Find tables
    Table* table1 = this->FindTable(table1Name);
    if (table1 == nullptr)
    {
        return;
    }
    Table* table2 = this->FindTable(table2Name);
    if (table2 == nullptr)
    {
        return;
    }
    //Inner join the two tables
    Table joinResult = Table::InnerJoin(*table1, col1, *table2, col2);
    //Add the join result to database's tables
    this->tables.Add(joinResult);
    this->tablesCount++;
}

void Database::RenameCommand(const Vector<String>& args)
{
    if (args.GetLength() < 3)
    {
        return;
    }
    //Extract table's old namd and table's new name
    String oldName = args[1];
    String newName = args[2];
    //Check if new name is unique
    Table* sameNameTable = this->FindTable(newName, false);
    if (sameNameTable != nullptr)
    {
        std::cerr << "Error: A table with name " << newName << " already exists." << std::endl;
        return;
    }
    //Find table
    Table* table = this->FindTable(oldName);
    if (table == nullptr)
    {
        return;
    }
    //Rename table
    table->SetName(newName);
}

void Database::CountCommand(const Vector<String>& args)
{
    if (args.GetLength() < 4)
    {
        return;
    }
    //Extract table's name, search column and seach value
    String tableName = args[1];
    unsigned searchColumn = args[2].ParseToInt();
    String searchValue = args[3];
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Count matching rows
    unsigned matchingRows = table->CountRows(searchColumn, searchValue);
    std::cout << "The number of matching rows is " << matchingRows << std::endl;
}

void Database::AggregateCommand(const Vector<String>& args)
{
    if (args.GetLength() < 6)
    {
        return;
    }
    //Extract all
    String tableName = args[1];
    unsigned searchCol = args[2].ParseToInt();
    String searchValue = args[3];
    unsigned targetCol = args[4].ParseToInt();
    AggregateOperation operation = Table::GetAggregateOperationFromString(args[5]);
    //Find table
    Table* table = this->FindTable(tableName);
    if (table == nullptr)
    {
        return;
    }
    //Aggregate table
    double result = table->Aggregate(targetCol, operation, searchCol, searchValue);
    std::cout << "Aggregate result: " << result << std::endl;
}

Table* Database::FindTable(const String& tableName, bool errorMessage)
{
    Table* foundTable = nullptr;
    for (int i = 0; i < this->tablesCount; i++)
    {
        if (this->tables[i].GetName() == tableName)
        {
            foundTable = &this->tables[i];
            break;
        }
    }
    if (foundTable == nullptr && errorMessage)
    {
        std::cerr << "Error: No table with name " << tableName << " is found." << std::endl;
    }
    return foundTable;
}