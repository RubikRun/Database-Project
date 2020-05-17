#pragma once

#include "../String/String.hpp"
#include "../Vector/Vector.hpp"
#include "../Table/Table.hpp"

#include <iostream>
using namespace std;

class Database
{
    //The name of the database and the name of the file where it's stored
    String name, filename;
    //The tables in the database 
    Vector<Table> tables;
    unsigned tablesCount;

    //Creates an empty database
    Database();

    public:

        //Reads a database from a file
        static Database ReadDatabaseFromFile(const String& filename);

        //Executes the command on the database
        void ExecuteCommand(const String& command);

    private:

        //Executes an import command
        void ImportCommand(const Vector<String>& args);

        //Executes a showtables command
        void ShowtablesCommand();

        //Executes a describe command
        void DescribeCommand(const Vector<String>& args);

        //Executes a print command
        void PrintCommand(const Vector<String>& args);

        //Executes an export command
        void ExportCommand(const Vector<String>& args);

        //Executes a select command
        void SelectCommand(const Vector<String>& args);

        //Executes an addcolumn command
        void AddcolumnCommand(const Vector<String>& args);

        //Executes an update command
        void UpdateCommand(const Vector<String>& args);

        //Executes a delete command
        void DeleteCommand(const Vector<String>& args);

        //Executes an insert command
        void InsertCommand(const Vector<String>& args);

        //Executes an innerjoin command
        void InnerjoinCommand(const Vector<String>& args);

        //Executes a rename command
        void RenameCommand(const Vector<String>& args);

        //Executes a count command
        void CountCommand(const Vector<String>& args);

        //Executes an aggregate command
        void AggregateCommand(const Vector<String>& args);

        //Finds the table with the given name
        Table* FindTable(const String& tableName, bool errorMessage = true);
};