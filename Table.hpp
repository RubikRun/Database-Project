#include "String/String.hpp"
#include "Vector/Vector.hpp"
#include "Enums.hpp"

#include <iostream>

typedef Vector<String> Row;

const unsigned INVALID_COLUMN = 0xffffffff;
const unsigned CELL_WIDTH = 20;
const char CELL_FILL_CHAR = ' ';

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

        //Creates a nicely formatted string for each column's name
        String GetColsString();

        //Creates a nicely formatted string for the requested row, and returns it
        String GetRowString(unsigned row);

        //Returns the indecies of all rows that have a given value in a given column
        //If no column specified, returns all rows' indecies
        Vector<unsigned> SelectIndecies(unsigned searchCol = INVALID_COLUMN, String searchValue = "");

        //Find all rows that have a given value in a given column
        //and view them with a string viewer
        void SelectAndView(unsigned searchCol = INVALID_COLUMN, String searchValue = "");
};

Table ReadTable(std::istream& stream);

Table ReadTableFromFile(String filename);