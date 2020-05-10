#pragma once

#include "Vector.hpp"

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

        //Returns the string's length
        unsigned GetLength() const;

        //Checks if the string is null
        bool IsNull() const;

        //Assigns the value of another string
        String& operator=(const String& another);

        //Compares this string to another string
        bool operator==(const String& another) const;

        //Concatenates this string with another string and returns the result
        String operator+(const String& another) const;
        //Concatenates a char array with a string and returns the result
        friend String operator+(const char* charArray, const String& another);

        //Appends the other string to the end of this string
        String& operator+=(const String& another);

        //Reads from the stream to the string until it reaches the separator or a new line
        String& ReadNext(std::istream& stream = std::cin, char separator = ' ', bool endOnNewLine = true);

        //Reads a line from the stream to the string
        String& ReadLine(std::istream& stream = std::cin);

        //Reads a word from the stream to the string
        String& ReadWord(std::istream& stream = std::cin);

        //Reads a line from the stream to the string, using the ">>" operator
        friend std::istream& operator>>(std::istream& stream, String& string);

        //Writes the string to the stream
        void Write(std::ostream& stream = std::cout) const;

        //Writes the string to the stream, using the "<<" operator
        friend std::ostream& operator<<(std::ostream& stream, String string);

        //Checks if the string is numerical, can be parsed to a numerical type
        bool IsNumerical() const;

        //Checks if the string can be parsed to an int
        bool IsInt() const;

        //Checks if the string can be parsed to a double
        bool IsDouble() const;

        //Parses the string to an int
        int ParseToInt() const;

        //Parses an int to a string
        friend String ParseFromInt(int n);

        //Parses the string to a double
        double ParseToDouble() const;

        //Parses a double to a string
        friend String ParseFromDouble(double d);

        //Extracts a substring specified with a begin index and a length
        String GetSubstring(unsigned begin, unsigned length) const;

        //Splits the string by a separator and returns a vector of the resulting parts
        Vector<String> Split(char separator = ' ') const;

        //Checks if the string starts with a given prefix
        bool StartsWith(String prefix) const;

        //Checks if the string ends with a given suffix
        bool EndsWith(String suffix) const;

        //Counts the occurrences of a char in the string
        unsigned Count(char c) const;

        //Finds the first occurrence of a char in the string and returns its index
        int Find(char c) const;

        //Frees the memory, makes the string null
        ~String();
};