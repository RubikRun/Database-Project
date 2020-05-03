#pragma once

#include "Basic.hpp"

#include <iostream>
#include <cstring>

class String
{
    //The char array where we keep the string
    char* charArray;
    unsigned length;
    
    public:
        //Creates a null string
        String();

        //Creates a string from a char array
        String(const char* charArray);

        //Creates a string from another string
        String(const String& another);

        //Assigns the value of a char array
        String& operator=(const char* charArray);

        //Assigns the value of another string
        String& operator=(const String& another);

        //Compares this string to a char array
        bool operator==(const char* charArray);

        //Compares this string to the other string
        bool operator==(const String& another);

        //Concatenates the string with a char array and returns the result
        String operator+(const char* charArray);

        //Concatenates a char array with a string and returns the result
        friend String operator+(const char* charArray, String string);

        //Concatenates this string and another string and returns the result
        String operator+(const String& another);

        //Appends a char array to the end of the string
        String operator+=(const char* charArray);

        //Appends the other string to the end of this string
        String operator+=(const String& another);

        //Reads from the stream until it reaches the separator or a new line
        void ReadNext(std::istream& stream = std::cin, char separator = ' ', bool endOnNewLine = true);

        //Reads a line from the stream
        void ReadLine(std::istream& stream = std::cin);

        //Reads a word from the stream
        void ReadWord(std::istream& stream = std::cin);

        //Reads a line from a stream to the string, using the ">>" operator
        friend std::istream& operator>>(std::istream& stream, String& string);

        //Writes the string to a stream
        void Write(std::ostream& stream = std::cout);

        //Writes the string to a stream, using the "<<" operator
        friend std::ostream& operator<<(std::ostream& stream, String string);

        //Checks if the string is numberical, can be parsed to a numerical type
        bool IsNumerical();

        //Parses the string to an int
        int ParseToInt();

        //Parses an int to a string
        friend String ParseFromInt(int n);

        //Parses the string to a double
        double ParseToDouble();

        //Parses a double to a string
        friend String ParseFromDouble(double d);

        //Extracts a substring specified with a begin index and a length
        String GetSubstring(unsigned begin, unsigned length);

        //Counts the occurrences of a char in the string
        unsigned Count(char c);

        //Finds the first occurrence of a char and returns its index
        int Find(char c);

        //Checks if the string is null
        bool IsNull();

        //Frees the memory
        ~String();
};

String::String()
{
    //Create a null string
    this->charArray = nullptr;
    this->length = 0;
}

String::String(const char* charArray)
{
    //First make the string null
    this->charArray = nullptr;
    //Assign the value of the char array
    *this = charArray;
}

String::String(const String& another)
{
    //First make the string null
    this->charArray = nullptr;
    //Assign the other string
    *this = another;
}

String& String::operator=(const char* charArray)
{
    //Destroy the string, in case it wasn't null
    this->~String();
    //If the char array is null, create a null string
    if (charArray == nullptr)
    {
        this->charArray = nullptr;
        this->length = 0;
    }
    //Otherwise copy the char array
    else
    {
        this->length = strlen(charArray);
        this->charArray = new char[this->length + 1];
        strcpy(this->charArray, charArray);
    }
    //Return this string, so that we can do multiple assignment
    return *this;
}

String& String::operator=(const String& another)
{
    //If this string is the other string, then do nothing
    if (this == &another)
    {
        //nothing
    }
    //Otherwise assign the other string's char array
    else
    {
        *this = another.charArray;
    }
    //Return this string, so that we can do multiple assignment
    return *this;
}

bool String::operator==(const char* charArray)
{
    //If either the string or the char array is null, then they are not equal
    if (this->IsNull() || charArray == nullptr)
    {
        return false;
    }
    //Otherwise compare the string's char array and the given char array
    return (strcmp(this->charArray, charArray) == 0);
}

bool String::operator==(const String& another)
{
    //Compare this string with the other string's char array
    return (*this == another.charArray);
}

bool String::IsNull()
{
    //Check if the char array is a null pointer
    return (this->charArray == nullptr);
}

String::~String()
{
    //Free the memory of the char array, if not null
    if (!this->IsNull())
    {
        delete[] this->charArray;
    }
}