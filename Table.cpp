#include "Table.hpp"

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
    const int CELLWIDTH = 10;

    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Filename: " << this->filename << std::endl;
    std::cout << "Number of rows: " << this->rowsCount << std::endl;
    std::cout << "Number of columns: " << this->colsCount << std::endl;
    std::cout << std::endl;

    std::cout << std::left << std::setw(CELLWIDTH) << "";
    for (int i = 0; i < colsCount; i++)
    {
         std::cout << std::left << std::setw(CELLWIDTH) << this->colNames[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < (colsCount + 1) * CELLWIDTH; i++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < rowsCount; i++)
    {
        std::cout << std::left << std::setw(CELLWIDTH) << i + 1;
        for (int j = 0; j < colsCount; j++)
        {
             std::cout << std::left << std::setw(CELLWIDTH) << this->rows[i][j];
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