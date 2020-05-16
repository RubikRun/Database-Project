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
        void ImportCommand(const String& command);

        //Executes a showtables command
        void ShowtablesCommand();
};