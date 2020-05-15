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
const String NOVALUE_EXTERNAL = "None";
const String NOVALUE_INTERNAL = "";
const unsigned VIEWMODE_LINESPERPAGE = 15;
const String INNERJOIN_PREFIX = "innerjoin_";
const String TABLENAME_SEPARATOR = "_";

class Table
{
    public:
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

        //Creates an empty table with a given name
        Table(const String& name);

        //Creates a table with a given name, filename, names and types of columns, and rows
        Table(const String& name, const String& filename,
            const Vector<String>& colNames, const Vector<ValueType>& colTypes, const Vector<Row>& rows);

        //Returns the name of the table
        const String& GetName();

        //Prints all info about the table (testing only)
        void PrintAll();

        //Reads a table from a file and returns it
        static Table ReadTableFromFile(String filename);

        //Writes the table to a file
        void WriteToFile(String filename) const;

        //Find all rows that have a given value in a given column
        //and view them with a string viewer
        void SelectAndView(unsigned searchCol = INVALID_COLUMN, String searchValue = "") const;

        //Adds a new column with a given name and type to the table
        void AddColumn(const String& colName, ValueType colType);

        //Finds all the rows that have the search value in the search column,
        //and updates their target column to have the the target value
        //if no target value specified, it will delete the values in the column
        //if search column is invalid, it will update all rows
        void UpdateRows(unsigned searchCol, const String& searchValue,
            unsigned targetCol, const String& targetValue = NOVALUE_INTERNAL);

        //Finds all the rows that have the search value in the search column, and deletes them.
        //If search column is invalid, it will delete all rows
        void DeleteRows(unsigned searchCol, const String& searchValue);

        //Adds a new row with the given values to the table
        //If some values are not specified, they will be set to novalue
        void AddRow(const Row& row = Row());

        //Inner joins two tables.
        //Finds each pair of rows (row1, row2), where row1 is from table1 and row2 is from table2,
        //and row1 in column1 has the same value as row2 in column2.
        //Creates a new table from those row pairs and returns it
        static Table InnerJoin(const Table& table1, unsigned column1, const Table& table2, unsigned column2);

    private:

        //Checks if the talbe is empty
        bool IsEmpty() const;

        //Reads a table from a stream and returns it
        static Table ReadTable(std::istream& stream, const char separator);

        //Writes the table to a stream
        void Write(std::ostream& stream, const char separator) const;

        //For each column finds the minimum width so that the given rows can be displayed,
        //without cells touching or overlapping
        Vector<unsigned> GetColWidths(Vector<unsigned> rowIndecies) const;

        //Creates a nicely formatted string for each column's name
        String GetColsString(Vector<unsigned> colWidths = Vector<unsigned>()) const;

        //Creates a nicely formatted string for the requested row, and returns it
        String GetRowString(unsigned row, Vector<unsigned> colWidths = Vector<unsigned>()) const;

        //Finds all the rows that have the search value in the search column, and returns their indecies
        //If no column specified or column is invalid, returns all rows' indecies
        //We can specify complement = true, if we want to find the rows that DON'T have the search value
        Vector<unsigned> SelectIndecies(unsigned searchCol = INVALID_COLUMN, const String& searchValue = "", 
            bool complement = false) const;
};