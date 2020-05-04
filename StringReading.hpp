#pragma once

#include "String.hpp"

void String::ReadNext(std::istream& stream, char separator, bool endOnNewLine)
{
    //Destroy the string, in case it's not null
    this->~String();
    //Initialize the string as an empty string
    this->charArray = new char[1];
    this->charArray[0] = '\0';
    this->length = 0;

    //Read characters one by one from the stream and add them to the string,
    //until we reach the separator or a new line
    char newChar;
    while ((stream >> std::noskipws >> newChar) && newChar != separator && (!endOnNewLine || newChar != '\n'))
    {
        //Create a buffer, copy the string to it and add the new character
        char* buffer = new char[this->length + 2];
        for (int i = 0; i < this->length; ++i)
        {
            buffer[i] = this->charArray[i];
        }
        buffer[this->length] = newChar;
        buffer[this->length + 1] = '\0';

        //The string becomes the buffer with the added character
        delete[] this->charArray;
        this->charArray = buffer;
        this->length++;
    }
}

void String::ReadLine(std::istream& stream)
{
    //Read from stream until we reach a new line
    this->ReadNext(stream, '\n');
}

void String::ReadWord(std::istream& stream)
{
    //Read from stream until we reach a space or a new line
    this->ReadNext(stream, ' ', true);
}

std::istream& operator>>(std::istream& stream, String& string)
{
    //Read a line from the stream
    string.ReadLine(stream);
    //Return the stream, so that we can do multiple operators
    return stream;
}