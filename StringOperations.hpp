#pragma once

#include "String.hpp"
#include "Vector.hpp"

String String::operator+(const char* charArray)
{
    //If the string is null, return a string made from the char array
    if (this->IsNull())
    {
        String string(charArray);
        return string;
    }
    //If the char array is null, return the string
    if (charArray == nullptr)
    {
        return *this;
    }

    //Create a char array for the concatenation
    unsigned arrLength = strlen(charArray);
    char* concat = new char[this->length + arrLength + 1];
    //Add the characters of the string to the concatenation
    for (int i = 0; i < this->length; ++i)
    {
        concat[i] = this->charArray[i];
    }
    //followed by the characters of the char array
    for (int i = 0; i < arrLength; ++i)
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
    //Return the concatenation
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
    //Return this string, so that we can use it in expressions
    return *this;
}

String String::operator+=(const String& another)
{
    //Find the concatenation and assign it to the string
    String concat = *this + another;
    *this = concat;
    //Return this string, so that we can use it in expressions
    return *this;
}

String String::GetSubstring(unsigned begin, unsigned length)
{
    //If the substring begins after the full string ends,
    //then return an empty string
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

Vector<String> String::Split(char separator)
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
            //The current part is from its beginning to right before the separator
            unsigned partLength = i - partBegin;
            String part = this->GetSubstring(partBegin, partLength);
            splitted.Add(part);
            //Set the next part's beginning to be right after the separator
            partBegin = i + 1;
        }
    }
    //The last part ends not on a separator, but on the end of the string
    String lastPart = this->GetSubstring(partBegin, this->length - partBegin);
    splitted.Add(lastPart);
    //Return the splitted string, a vector of the resulting parts
    return splitted;
}

bool String::StartsWith(String prefix)
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

bool String::EndsWith(String suffix)
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

unsigned String::Count(char c)
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

int String::Find(char c)
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