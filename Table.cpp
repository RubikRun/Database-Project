#include "Table.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#define tabOut cout << left << setw(CELLWIDTH)

void Table::Print() const
{
    const int CELLWIDTH = 10;


    cout << "Name: " << this->name << endl;
    cout << "Filename: " << this->filename << endl;
    cout << "Number of rows: " << this->rowsCount << endl;
    cout << "Number of columns: " << this->colsCount << endl;
    cout << endl;

    tabOut << "";
    for (int i = 0; i < colsCount; i++)
    {
        tabOut << this->colNames[i];
    }
    cout << endl;

    for (int i = 0; i < rowsCount; i++)
    {
        tabOut << i + 1;
        for (int j = 0; j < colsCount; j++)
        {
            tabOut << this->values[i][j];
        }
        cout << endl;
    }
}

Table::Table(String name, String filename)
{
    this->rowsCount = this->colsCount = 0;
    
    this->name = name;
    this->filename = filename;
}

Table::Table(String name, String filename, const Vector<String>& colNames,
        const Vector<ValueType>& colTypes, const Vector< Vector<String> >& values)
{
    this->rowsCount = values.GetLength();
    this->colsCount = colTypes.GetLength();

    this->colNames = colNames;
    this->colTypes = colTypes;

    this->values = values;

    this->name = name;
    this->filename = filename;
}

Table ReadTableFromFile(String filename)
{
    //Open the file
    std::ifstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Read table's name
    String tableName;
    tableName.ReadWord(file);

    //Create a table
    Table table(tableName, filename);

    //Read the number of table's rows and columns
    table.rowsCount = ReadInt(file);
    table.colsCount = ReadInt(file);

    //Read columns' names and types
    ReadColsInfoFromFile(file, table);

    //Read values of the table's cells into a 2d array
    ReadValuesFromFile(file, table);

    //Close the file
    file.close();
    if (file.is_open())
    {
        std::cerr << "Error: Cannot close file: " << filename << std::endl;
    }

    //Return the table
    return table;
}

void ReadColsInfoFromFile(std::ifstream& file, Table& table)
{
    //Create vectors for the columns' names and types
    table.colNames = Vector<String>(table.colsCount, "");
    table.colTypes = Vector<ValueType>(table.colsCount, Whole);
    //Read columns' names and types
    for (int i = 0; i < table.colsCount; i++)
    {
        //Read column name
        table.colNames[i].ReadWord(file);
        //Read column type
        int typeIndex = ReadInt(file);
        table.colTypes[i] = (ValueType)typeIndex;
    }
}

void ReadValuesFromFile(std::ifstream& file, Table& table)
{
    //Create a 2D vector for the values
    table.values = Vector< Vector<String> >(table.rowsCount, Vector<String>());
    for (int i = 0; i < table.rowsCount; i++)
    {
        table.values[i] = Vector<String>(table.colsCount, "");
    }
    //Read values
    for (int i = 0; i < table.rowsCount; i++)
    {
        for (int j = 0; j < table.colsCount; j++)
        {
            table.values[i][j].ReadWord(file);
        }
    }
}

void Table::WriteToFile(String filename) const
{
    std::ofstream file(filename.GetCharArray());
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
    }

    //Write table's name
    file << this->name << " ";
    //Write the number of rows and cols in table
    file << this->rowsCount << " " << this->colsCount << " ";

    //Write columns' names and types
    for (int i = 0; i < this->colsCount; i++)
    {
        file << this->colNames[i] << " " << this->colTypes[i] << " ";
    }

    //Write cells' values
    for (int i = 0; i < this->rowsCount; i++)
    {
        for (int j = 0; j < this->colsCount; j++)
        {
            file << this->values[i][j] << " ";
        }
    }
}

void Table::SetName(const String& givenName)
{
    this->name = givenName;
}