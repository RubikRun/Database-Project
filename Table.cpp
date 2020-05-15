#include "Table.hpp"
#include "String/StringViewer.hpp"

#include <fstream>
#include <iomanip>

Table::Table(const String& name)
{
    this->name = name;

    this->rowsCount = this->colsCount = 0;
}

Table::Table(const String& name, const String& filename,
const Vector<String>& colNames, const Vector<ValueType>& colTypes, const Vector<Row>& rows)
{
    this->name = name;
    this->filename = filename;
    this->colNames = colNames;
    this->colTypes = colTypes;
    this->rows = rows;

    this->rowsCount = rows.GetLength();
    this->colsCount = colNames.GetLength();
}

const String& Table::GetName()
{
    return this->name;
}

void Table::PrintAll()
{
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Filename: " << this->filename << std::endl;
    std::cout << "Number of rows: " << this->rowsCount << std::endl;
    std::cout << "Number of columns: " << this->colsCount << std::endl;
    std::cout << std::endl;

    std::cout << std::left << std::setw(CELL_WIDTH) << "";
    for (int i = 0; i < colsCount; i++)
    {
         std::cout << std::left << std::setw(CELL_WIDTH) << this->colNames[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < (colsCount + 1) * CELL_WIDTH; i++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < rowsCount; i++)
    {
        std::cout << std::left << std::setw(CELL_WIDTH) << i + 1;
        for (int j = 0; j < colsCount; j++)
        {
             std::cout << std::left << std::setw(CELL_WIDTH) << this->rows[i][j];
        }
        std::cout << std::endl;
    }
}

//Reads columns' names and types from a stream
static void ReadColsInfo(Vector<String>& colNames, Vector<ValueType>& colTypes,
    unsigned colsCount, std::istream& stream, const char separator)
{
    //We know the number of names and types beforehand,
    //so we can create the two vectors and then read to them
    colNames = Vector<String>(colsCount, "");
    colTypes = Vector<ValueType>(colsCount, Text);

    //Read names and types
    for (int i = 0; i < colsCount; i++)
    {
        colNames[i].ReadNext(stream, separator);

        int typeIndex = String::ReadInt(stream, separator);
        colTypes[i] = (ValueType)typeIndex;
    }
}

//Reads rows from a stream
static void ReadRows(Vector<Row>& rows, unsigned rowsCount, unsigned colsCount, std::istream& stream, const char separator)
{
    //We know the number of rows and columns beforehand,
    //so we can create the vector of rows, and for each row create the vector of cols,
    //and then read to them
    rows = Vector<Row>(rowsCount, Row());
    for (int i = 0; i < rowsCount; i++)
    {
        rows[i] = Row(colsCount, "");
    }

    //Read rows
    for (int i = 0; i < rowsCount; i++)
    {
        for (int j = 0; j < colsCount; j++)
        {
            rows[i][j].ReadNext(stream, separator);
        }
    }
}

Table Table::ReadTable(std::istream& stream, const char separator)
{
    //Read table's name
    String name;
    name.ReadNext(stream, FILE_SEPARATOR);

    //Read number of rows and columns of the table
    unsigned rowsCount = String::ReadInt(stream, FILE_SEPARATOR);
    unsigned colsCount = String::ReadInt(stream, FILE_SEPARATOR);

    //Read columns' names and types
    Vector<String> colNames;
    Vector<ValueType> colTypes;
    ReadColsInfo(colNames, colTypes, colsCount, stream, FILE_SEPARATOR);

    //Read rows
    Vector<Row> rows;
    ReadRows(rows, rowsCount, colsCount, stream, FILE_SEPARATOR);

    //Create a table with the data that we read
    Table table(name, "", colNames, colTypes, rows);

    return table;
}

Table Table::ReadTableFromFile(String filename)
{
    //Open file
    std::ifstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Read table from file
    Table table = Table::ReadTable(file, FILE_SEPARATOR);

    //Set table's filename
    table.filename = filename;

    //Close file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot close file: " << filename << std::endl;
    }

    return table;
}

void Table::Write(std::ostream& stream, const char separator) const
{
    //Write table's name
    stream << this->name << separator;
    //Write the number of rows and columns
    stream << this->rowsCount << separator << this->colsCount << separator;

    //Write names and types of columns
    for (int i = 0; i < this->colsCount; i++)
    {
        stream << this->colNames[i] << separator << this->colTypes[i] << separator;
    }

    //Write rows
    for (int i = 0; i < this->rowsCount; i++)
    {
        for (int j = 0; j < this->colsCount; j++)
        {
            stream << this->rows[i][j] << separator;
        }
    }
}

void Table::WriteToFile(String filename) const
{
    //Open file
    std::ofstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Write table to file
    this->Write(file, FILE_SEPARATOR);

    //Close file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot close file: " << filename << std::endl;
    }
}

Vector<unsigned> Table::GetColWidths(Vector<unsigned> rowIndecies) const
{
    Vector<unsigned> colWidths(this->colsCount + 1, 0);

    //First column will contain row numbers, so it needs to be as wide as to contain the largest row number
    colWidths[0] = String::ParseFromInt(this->rowsCount + 1).GetLength() + CELL_OFFSET_MIN;
    //For each table column we will go through the rows we need and find the longest value (+ the column name)
    for (int i = 0; i < this->colsCount; i++)
    {
        colWidths[i + 1] = this->colNames[i].GetLength() + CELL_OFFSET_MIN;
        for (int j = 0; j < rowIndecies.GetLength(); j++)
        {
            unsigned currRowIndex = rowIndecies[j];
            String currValue = this->rows[currRowIndex][i];
            unsigned currValueLen = currValue.GetLength();

            if (currValueLen + CELL_OFFSET_MIN > colWidths[i + 1])
            {
                colWidths[i + 1] = currValueLen + CELL_OFFSET_MIN;
            }
        }
    }

    return colWidths;
}

String Table::GetColsString(Vector<unsigned> colWidths) const
{
    //If cells' widths are not specified, every cell will be with a constant width
    if (colWidths.IsEmpty())
    {
        colWidths = Vector<unsigned>(this->colsCount + 1, CELL_WIDTH);
    }

    //Put an empty cell in the beginning, for the column of row numbers
    String colsString = String(CELL_FILL_CHAR, colWidths[0]);

    //Put columns' names
    for (int i = 0; i < this->colsCount; i++)
    {
        //Put current column's name
        String currColName = this->colNames[i];
        colsString += currColName;
        //Fill the remaining cell
        unsigned charsLeft = colWidths[i + 1] - currColName.GetLength();
        colsString += String(CELL_FILL_CHAR, charsLeft);
    }

    return colsString;
}

String Table::GetRowString(unsigned row, Vector<unsigned> colWidths) const
{
    //Check if the requested row is valid
    if (row >= this->rowsCount)
    {
        return String();
    }

    //If cells' widths are not specified, every cell will be with a constant width
    if (colWidths.IsEmpty())
    {
        colWidths = Vector<unsigned>(this->colsCount + 1, CELL_WIDTH);
    }

    //Put the row's number first
    String rowString = String::ParseFromInt(row + 1);
    //Fill the remaining cell
    unsigned charsLeft = colWidths[0] - rowString.GetLength();
    rowString += String(CELL_FILL_CHAR, charsLeft);

    //Put the value of each column
    for (int i = 0; i < this->colsCount; i++)
    {
        //Put current column's value
        String currValue = this->rows[row][i];
        if (currValue == NOVALUE_INTERNAL)
        {
            currValue = NOVALUE_EXTERNAL;
        }
        rowString += currValue;
        //Fill the remaining cell
        charsLeft = colWidths[i + 1] - currValue.GetLength();
        rowString += String(CELL_FILL_CHAR, charsLeft);
    }

    return rowString;
}

Vector<unsigned> Table::SelectIndecies(unsigned searchCol, const String& searchValue, bool complement) const
{
    //Check is the given column is valid
    if (searchCol >= this->colsCount)
    {
        searchCol = INVALID_COLUMN;
    }

    //Find all rows with search value in the search column
    //or if search column is invalid, just add all rows
    Vector<unsigned> foundRows;
    for (int i = 0; i < this->rowsCount; i++)
    {
        if (searchCol == INVALID_COLUMN ||
        (this->rows[i][searchCol] == searchValue) != complement )
        {
            foundRows.Add(i);
        }
    }

    return foundRows;
}

void Table::SelectAndView(unsigned searchCol, String searchValue) const
{
    //Find the rows
    Vector<unsigned> rowIndecies = this->SelectIndecies(searchCol, searchValue);

    //Find columns' widths
    Vector<unsigned> colWidths = this->GetColWidths(rowIndecies);

    //Create a string of all the rows
    String rowsString = "";
    for (int i = 0; i < rowIndecies.GetLength(); i++)
    {
        unsigned currRowIndex = rowIndecies[i];
        String currRowString = this->GetRowString(currRowIndex, colWidths);
        rowsString += currRowString;

        if (i < rowIndecies.GetLength() - 1)
        {
            rowsString += "\n";
        }
    }

    //View rows with a string viewer
    String colsString = this->GetColsString(colWidths) + "\n";
    StringViewer stringViewer(rowsString, VIEWMODE_LINESPERPAGE, colsString);
    stringViewer.ViewMode();
}

void Table::AddColumn(const String& colName, ValueType colType)
{
    //Increase the number of columns in the table
    this->colsCount++;
    //Add the new column's name and type to the columns' names and types
    this->colNames.Add(colName);
    this->colTypes.Add(colType);
    //Each row will have no value in the new column,
    //so we need to add one more value - no value - to each row
    for (int i = 0; i < this->rowsCount; i++)
    {
        this->rows[i].Add(NOVALUE_INTERNAL);
    }
}

void Table::UpdateRows(unsigned searchCol, const String& searchValue,
        unsigned targetCol, const String& targetValue)
{
    //Check if target column is valid
    if (targetCol >= this->colsCount)
    {
        std::cerr << "Error: Not a valid target column. The table has columns from 1 to " << this->colsCount << std::endl;
        return;
    }

    //Find the rows with the search value in the search column
    Vector<unsigned> foundRows = this->SelectIndecies(searchCol, searchValue);

    //Update rows' target columns
    for (int i = 0; i < foundRows.GetLength(); i++)
    {
        unsigned rowIndex = foundRows[i];
        this->rows[rowIndex][targetCol] = targetValue;
    }
}

void Table::DeleteRows(unsigned searchCol, const String& searchValue)
{
    //Find the rows that don't have the search value in the search value,
    //so the rows that we want to keep. ALl other rows should be deleted
    Vector<unsigned> foundRows = this->SelectIndecies(searchCol, searchValue, true);
    if (foundRows.GetLength() == 0)
    {
        return;
    }

    //Find the rows that we want to keep
    Vector<Row> rowsAfterDelete = Vector<Row>(foundRows.GetLength(), Row());
    for (int i = 0; i < foundRows.GetLength(); i++)
    {
        unsigned rowIndex = foundRows[i];
        Row currRow = this->rows[rowIndex];
        rowsAfterDelete[i] = (currRow);
    }

    //Set the table's rows to be the rows that we want to keep
    this->rows = rowsAfterDelete;
    this->rowsCount = this->rows.GetLength();
}

void Table::AddRow(const Row& rowValues)
{
    //Create a new row with the needed length
    Row newRow = Row(this->colsCount, NOVALUE_INTERNAL);
    //Fill the new row with the values given
    for (int i = 0; i < this->colsCount; i++)
    {
        //If we have no values left, stop filling
        if (i >= rowValues.GetLength())
        {
            break;
        }
        //Don't allow null strings. A null string will be novalue
        if (!rowValues[i].IsNull())
        {
            newRow[i] = rowValues[i];
        }
    }

    //Add the new row to table's rows
    this->rows.Add(newRow);
    this->rowsCount++;
}

bool Table::IsEmpty() const
{
    return (this->rowsCount == 0);
}

Table Table::InnerJoin(const Table& table1, unsigned column1, const Table& table2, unsigned column2)
{
    String joinName = INNERJOIN_PREFIX + table1.name + TABLENAME_SEPARATOR + table2.name;

    //If either one of the tables is empty, the join will be empty
    if (table1.IsEmpty() || table2.IsEmpty())
    {
        return Table(joinName);
    }

    //Create the join empty
    Table join = Table(joinName);
    //Add column names and types to join
    join.colNames = table1.colNames + table2.colNames;
    join.colTypes = table1.colTypes + table2.colTypes;
    join.colsCount = table1.colNames.GetLength() + table2.colNames.GetLength();
    //Find the rows and add them to the join
    for (int i = 0; i < table1.rowsCount; i++)
    {
        for (int j = 0; j < table2.rowsCount; j++)
        {
            if (table1.rows[i][column1] == table2.rows[j][column2])
            {
                Row joinRow = table1.rows[i] + table2.rows[j];
                join.AddRow(joinRow);
            }
        }
    }

    return join;
}