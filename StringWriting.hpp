#pragma once

#include "String.hpp"
#include "StringViewer.hpp"

void String::Write(std::ostream& stream) const
{
    //If the string is not null, write it to the stream
    if (!this->IsNull())
    {
        stream << this->charArray;
    }
}

std::ostream& operator<<(std::ostream& stream, String string)
{
    //Write the string to the stream
    string.Write(stream);
    //Return the stream, so that we can do multiple operators
    return stream;
}