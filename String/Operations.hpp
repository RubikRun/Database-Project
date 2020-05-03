#pragma once

#include "String.hpp"

String String::operator+(const char* charArray)
{
    //If the string is null, return a string made from the char array
    if (this->IsNull())
    {
        String string(charArray);
        return string;
    }
    //If the char array is null, return this string
    if (charArray == nullptr)
    {
        return *this;
    }

    //Create a char array for the concatenation
    unsigned arrLength = strlen(charArray);
    char* concat = new char[this->length + arrLength + 1];
    //Add the characters of the string
    for (int i = 0; i < this->length; i++)
    {
        concat[i] = this->charArray[i];
    }
    //followed by the characters of the char array
    for (int i = 0; i < arrLength; i++)
    {
        concat[this->length + i] = charArray[i];
    }
    concat[this->length + arrLength] = '\0';

    //Create a string from the concatenation and free the memory
    String string(concat);
    delete[] concat;

    return string;
}

String operator+(const char* charArray, String string)
{
    //Create a string from the char array
    String arrString(charArray);
    //Return the concatenation of the string from the char array and the given string
    return arrString + string;
}

String String::operator+(const String& another)
{
    //Concatenate this string and the other string's char array
    return *this + another.charArray;
}

String String::operator+=(const char* charArray)
{
    //Find the concatenation and assign it to the string
    String concat = *this + charArray;
    *this = concat;

    return *this;
}

String String::operator+=(const String& another)
{
    //Find the concatenation and assign it to the string
    String concat = *this + another;
    *this = concat;

    return *this;
}

String String::GetSubstring(unsigned begin, unsigned length)
{
    //If the substring begins after the full string ends,
    //then return an empty string
    if (begin >= this->length)
    {
        const char* emp = "";
        String string(emp);

        return emp;
    }
    //If the substring ends after the full string ends,
    //then reduce the length of the substring so that it ends where the full string ends
    if (begin + length >= this->length)
    {
        length = this->length - begin;
    }
    //Create a char array and fill it with the chars of the substring
    char* charArray = new char[length + 1];
    for (int i = 0; i < length; i++)
    {
        charArray[i] = this->charArray[begin + i];
    }
    charArray[length] = '\0';
    //Create a string from the char array
    String string(charArray);
    delete[] charArray;
    
    return string;
}

unsigned String::Count(char c)
{
    //Traverse all the chars in the string with a char pointer
    //Count the ones that are equal to the given char
    unsigned counter = 0;
    for (char* p = this->charArray; *p != '\0'; p++)
    {
        if (*p == c)
        {
            counter++;
        }
    }
    return counter;
}

int String::Find(char c)
{
    //Traverse all the chars in the string
    //If we reach a char equal to the given char, return its index
    for (int i = 0; i < this->length; i++)
    {
        if (this->charArray[i] == c)
        {
            return i;
        }
    }
    return -1;
}