#pragma once

#include "String/String.hpp"
#include "Vector/Vector.hpp"

#include "Enums.hpp"

#include <fstream>

class Table
{
    //The number of rows and columns in the table
    unsigned rowsCount, colsCount;

    //The names and types of table's columns
    Vector<String> colNames;
    Vector<ValueType> colTypes;

    //The values in the table's cells
    Vector< Vector<String> > values;

    //The name of the table and the name of the file where it's stored
    String name, filename;

    public:

        //Prints the whole table, every information about it (for testing only)
        void Print() const;

        void PrintRows() const;

        //Creates an empty table with a given name and filename
        Table(String name, String filename);

        //Creates a table with a given name and filename and fills it with the given values and column types and names
        Table(String name, String filename, const Vector<String>& colNames,
        const Vector<ValueType>& colTypes, const Vector< Vector<String> >& values);

        //Reads a table from a file
        friend Table ReadTableFromFile(String filename);

        //Writes a table to a file
        void WriteToFile(String filename) const;

        //Changes the table's name
        void SetName(const String& name);

        //Returns all the rows wich have a given value in a given column
        Vector< Vector<String> > Select(unsigned searchColIndex, String searchValue);

        //Counts the number of rows that have a given value in a given column
        unsigned CountRows(unsigned searchColIndex, String searchValue);

        //Adds a new column with given name and type to the table
        void AddColumn(String colName, ValueType colType);

        //Finds the rows of the table that have the search value in the search column
        //and updates their target column to have the taget value
        void UpdateRows(unsigned searchColIndex, String searchValue, unsigned targetColIndex, String targetValue);

        //Deletes all the rows of the table that have the search value in the search column
        void DeleteRows(unsigned searchColIndex, String searchValue);

        //Inserts a new row with the given values to the table
        void InsertRow(Vector<String>& rowValues);

        //Does inner join on the two given tables, returns the resulting table
        friend Table InnerJoin(const Table& table1, const Table& table2);

        //Does the operation on the values of a given column of all the rows that have the search value in the search column
        //Returns the result as a string
        String Aggregate(unsigned searchColIndex, String searchValue, unsigned targetColIndex, AggregateOperations operation);

    private:

        //Reads columns' names and types from a file to the given table
        friend void ReadColsInfoFromFile(std::ifstream& file, Table& table);

        //Reads cells' values from a file to the given table
        friend void ReadValuesFromFile(std::ifstream& file, Table& table);
};

Table ReadTableFromFile(String filename);

Table InnerJoin(const Table& table1, const Table& table2);

void ReadColsInfoFromFile(std::ifstream& file, Table& table);

void ReadValuesFromFile(std::ifstream& file, Table& table);