#pragma once

#include "String.hpp"
#include "Vector.hpp"

String String::operator+(const String& another) const
{
    //If this string is null or empty, return the other string
    if (this->IsNull() || this->length == 0)
    {
        return another;
    }
    //If the other string is null or empty, return this string
    if (another.IsNull() || another.length == 0)
    {
        return *this;
    }

    //Create a char array for the concatenation
    unsigned concatLength = this->length + another.length;
    char* concatArr = new char[concatLength + 1];
    //Copy this string to the concatenation
    for (int i = 0; i < this->length; i++)
    {
        concatArr[i] = this->charArray[i];
    }
    //followed by the other string
    for (int i = 0; i < another.length; i++)
    {
        concatArr[this->length + i] = another.charArray[i];
    }
    concatArr[concatLength] = '\0';

    //Create a string from the concatenation and free the memory
    String concat(concatArr);
    delete[] concatArr;

    return concat;
}

String operator+(const char* charArray, const String& string)
{
    //Create a string from the char array
    String arrString(charArray);
    //Concatenate the two strings
    String concat = arrString + string;

    return concat;
}

String& String::operator+=(const String& another)
{
    //Find the concatenation and assign it to the string
    String concat = *this + another;
    *this = concat;
    //Return this string, so that we can use it in expressions
    return *this;
}

String String::GetSubstring(unsigned begin, unsigned length) const
{
    //If the substring begins after the full string ends, then return an empty string
    if (begin >= this->length)
    {
        return "";
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
    //Create a string from the char array and free the memory
    String string(charArray);
    delete[] charArray;
    
    return string;
}

Vector<String> String::Split(char separator) const
{
    Vector<String> splitted;
    //If the string is null, return an empty vector
    if (this->IsNull())
    {
        return splitted;
    }
    //The first part begins at the beginning of the string
    unsigned partBegin = 0;
    //Traverse the string
    for (int i = 0; i < this->length; i++)
    {
        //If we reach a separator
        if (this->charArray[i] == separator)
        {
            //The current part should end right before the separator
            unsigned partLength = i - partBegin;
            String part = this->GetSubstring(partBegin, partLength);
            splitted.Add(part);
            //Set the next part's beginning to be right after the separator
            partBegin = i + 1;
        }
    }
    //The last part ends on the end of the string
    String lastPart = this->GetSubstring(partBegin, this->length - partBegin);
    splitted.Add(lastPart);
    //Return the splitted string, a vector of the resulting parts
    return splitted;
}

bool String::StartsWith(String prefix) const
{
    //If the string or the prefix is null, or the prefix is longer than the string,
    //then this cannot be a prefix
    if (this->IsNull() || prefix.IsNull() || prefix.length > this->length)
    {
        return false;
    }

    //Get the real prefix of the string - its prefix with the same length as the given prefix
    String realPrefix = this->GetSubstring(0, prefix.length);
    //Check if the real prefix is the same as the given prefix
    return (prefix == realPrefix);
}

bool String::EndsWith(String suffix) const
{
    //If the string or the suffix is null, or the suffix is longer than the string,
    //then this cannot be a suffix
    if (this->IsNull() || suffix.IsNull() || suffix.length > this->length)
    {
        return false;
    }

    //Get the real suffix of the string - its suffix with the same length as the given suffix
    String realSuffix = this->GetSubstring(this->length - suffix.length, suffix.length);
    //Check if the real prefix is the same as the given prefix
    return (suffix == realSuffix);
}

unsigned String::Count(char c) const
{
    unsigned counter = 0;
    //Traverse all the chars in the string with a char pointer
    for (char* p = this->charArray; *p != '\0'; p++)
    {
        //Count the ones that are euqual to the given char
        if (*p == c)
        {
            counter++;
        }
    }
    return counter;
}

int String::Find(char c) const
{
    //Traverse all the chars in the string with an index
    for (int i = 0; i < this->length; i++)
    {
        //The first time we reach a char eual to the given one, we return its index
        if (this->charArray[i] == c)
        {
            return i;
        }
    }
    //If we never reach an equal char, we return -1 (an invalid index)
    return -1;
}