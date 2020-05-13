#include "String/String.hpp"
#include "Vector/Vector.hpp"
#include "Enums.hpp"

#include <iostream>

typedef Vector<String> Row;

const unsigned UNSIGNED_MAX = 0xffffffff;
const unsigned INVALID_COLUMN = UNSIGNED_MAX;
const unsigned CELL_WIDTH = 20;
const char CELL_FILL_CHAR = ' ';
const char CELL_OFFSET_MIN = 2;
const char FILE_SEPARATOR = ',';

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

        //Reads a table from a file and returns it
        static Table ReadTableFromFile(String filename);

        //Writes the table to a file
        void WriteToFile(String filename);

        //Find all rows that have a given value in a given column
        //and view them with a string viewer
        void SelectAndView(unsigned searchCol = INVALID_COLUMN, String searchValue = "");

    private:

        //Reads a table from a stream and returns it
        static Table ReadTable(std::istream& stream, const char separator);

        //Writes the table to a stream
        void Write(std::ostream& stream, const char separator);

        //For each column finds the minimum width so that the given rows can be displayed,
        //without cells touching or overlapping
        Vector<unsigned> GetColWidths(Vector<unsigned> rowIndecies);

        //Creates a nicely formatted string for each column's name
        String GetColsString(Vector<unsigned> colWidths = Vector<unsigned>());

        //Creates a nicely formatted string for the requested row, and returns it
        String GetRowString(unsigned row, Vector<unsigned> colWidths = Vector<unsigned>());

        //Returns the indecies of all rows that have a given value in a given column
        //If no column specified, returns all rows' indecies
        Vector<unsigned> SelectIndecies(unsigned searchCol = INVALID_COLUMN, String searchValue = "");
};