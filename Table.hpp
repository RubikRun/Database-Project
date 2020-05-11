#include "String/String.hpp"
#include "Vector/Vector.hpp"
#include "Enums.hpp"

#include <iostream>

typedef Vector<String> Row;

class Table
{
    //The name of the table and the name of the file where it's stored
    String name, filename;

    //The number of rows and columns in the table
    unsigned rowsCount, colsCount;

    //The names and types of the columns
    Vector<String> colNames;
    Vector<ValueType> colTypes;

    //The rows in the table
    Vector<Row> rows;

    public:

        //Creates an empty table with a given name and filename
        Table(const String& name, const String& filename);

        //Creates a table with a given name, filename, names and types of columns, and rows
        Table(const String& name, const String& filename,
        const Vector<String>& colNames, const Vector<ValueType>& colTypes, const Vector<Row>& rows);

        //Prints all info about the table (testing only)
        void PrintAll();

        //Reads a table from a stream and returns it
        friend Table ReadTable(std::istream& stream);

        //Reads a table from a file and returns it
        friend Table ReadTableFromFile(String filename);

        //Writes the table to a stream
        void Write(std::ostream& stream);

        //Writes the table to a file
        void WriteToFile(String filename);
};

Table ReadTable(std::istream& stream);

Table ReadTableFromFile(String filename);