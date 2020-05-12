#include "Table.hpp"
#include "String/StringViewer.hpp"

#include <fstream>
#include <iomanip>

Table::Table(const String& name, const String& filename)
{
    this->name = name;
    this->filename = filename;

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
void ReadColsInfo(Vector<String>& colNames, Vector<ValueType>& colTypes,
unsigned colsCount, std::istream& stream)
{
    //We know the number of names and types beforehand,
    //so we can create the two vectors and then read to them
    colNames = Vector<String>(colsCount, "");
    colTypes = Vector<ValueType>(colsCount, Text);

    //Read names and types
    for (int i = 0; i < colsCount; i++)
    {
        colNames[i].ReadWord(stream);

        int typeIndex = ReadInt(stream);
        colTypes[i] = (ValueType)typeIndex;
    }
}

//Reads rows from a stream
void ReadRows(Vector<Row>& rows, unsigned rowsCount, unsigned colsCount, std::istream& stream)
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
            rows[i][j].ReadWord(stream);
        }
    }
}

Table ReadTable(std::istream& stream)
{
    //Read table's name
    String name;
    name.ReadWord(stream);

    //Read number of rows and columns of the table
    unsigned rowsCount = ReadInt(stream);
    unsigned colsCount = ReadInt(stream);

    //Read columns' names and types
    Vector<String> colNames;
    Vector<ValueType> colTypes;
    ReadColsInfo(colNames, colTypes, colsCount, stream);

    //Read rows
    Vector<Row> rows;
    ReadRows(rows, rowsCount, colsCount, stream);

    //Create a table with the data that we read
    Table table(name, "", colNames, colTypes, rows);

    return table;
}

Table ReadTableFromFile(String filename)
{
    //Open file
    std::ifstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Read table from file
    Table table = ReadTable(file);

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

void Table::Write(std::ostream& stream)
{
    //Write table's name
    stream << this->name << " ";
    //Write the number of rows and columns
    stream << this->rowsCount << " " << this->colsCount << " ";

    //Write names and types of columns
    for (int i = 0; i < this->colsCount; i++)
    {
        stream << this->colNames[i] << " " << this->colTypes[i] << " ";
    }

    //Write rows
    for (int i = 0; i < this->rowsCount; i++)
    {
        for (int j = 0; j < this->colsCount; j++)
        {
            stream << this->rows[i][j] << " ";
        }
    }
}

void Table::WriteToFile(String filename)
{
    //Open file
    std::ofstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Write table to file
    this->Write(file);

    //Close file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot close file: " << filename << std::endl;
    }
}

//Fills whats left of the last cell, so that it's exactly the width needed
void FillCell(String& str)
{
    if (str.GetLength() % CELL_WIDTH != 0)
    {
        unsigned charsLeft = CELL_WIDTH - str.GetLength() % CELL_WIDTH;
        str += String(CELL_FILL_CHAR, charsLeft);
    }
}

String Table::GetColsString()
{
    //Put an empty cell in the beginning, for the column of row numbers
    String colsString = String(CELL_FILL_CHAR, CELL_WIDTH);

    //Put columns' names
    for (int i = 0; i < this->colsCount; i++)
    {
        colsString += this->colNames[i];
        FillCell(colsString);
    }

    return colsString;
}

String Table::GetRowString(unsigned row)
{
    //Check if the requested row is valid
    if (row >= this->rowsCount)
    {
        return String();
    }

    //Put the row's number first
    String rowString = ParseFromInt(row + 1);
    FillCell(rowString);

    //Put the value of each column
    for (int i = 0; i < this->colsCount; i++)
    {
        rowString += this->rows[row][i];
        FillCell(rowString);
    }

    return rowString;
}

Vector<unsigned> Table::SelectIndecies(unsigned searchCol, String searchValue)
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
        if (searchCol == INVALID_COLUMN || this->rows[i][searchCol] == searchValue)
        {
            foundRows.Add(i);
        }
    }

    return foundRows;
}

void Table::SelectAndView(unsigned searchCol, String searchValue)
{
    //Find the rows
    Vector<unsigned> rowIndecies = this->SelectIndecies(searchCol, searchValue);

    //Create a string of all the rows
    String rowsString = "";
    for (int i = 0; i < rowIndecies.GetLength(); i++)
    {
        unsigned currRowIndex = rowIndecies[i];
        String currRowString = this->GetRowString(currRowIndex);
        rowsString += currRowString;

        if (i < rowIndecies.GetLength() - 1)
        {
            rowsString += "\n";
        }
    }

    //View rows with a string viewer
    String colsString = this->GetColsString() + "\n";
    StringViewer stringViewer(rowsString, colsString, 5);
    stringViewer.ViewMode();
}