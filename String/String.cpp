#include "String.hpp"
#include <cstring>

#include <iostream>
using namespace std;

const double EPSILON = 0.001;

bool IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

int ParseToDigit(char c)
{
    return c - '0';
}

char ParseFromDigit(int d)
{
    return d + '0';
}

int CountDigits(int n)
{
    int digits = 0;
    
    do
    {
        n /= 10;
        digits++;
    } while (n != 0);

    return digits;
}

template <typename T>
T absVal(T x)
{
    if (x < 0)
    {
        return (-x);
    }
    return x;
}

bool CloseEnough(double a, double b)
{
    return ( absVal(a - b) <= EPSILON );
}

String::String()
{
    //Create a null string
    this->charArray = nullptr;
    this->length = 0;
}

String::String(const char* charArray)
{
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
}

String::String(const String& another)
{
    //If the other string is null, create a null string
    if (another.IsNull())
    {
        this->charArray = nullptr;
        this->length = 0;
    }
    //Otherwise copy the other string
    else
    {
        this->length = another.length;
        this->charArray = new char[this->length + 1];
        strcpy(this->charArray, another.charArray);
    }
}

unsigned String::GetLength() const
{
    //Return the string's length
    return this->length;
}

const char* String::GetCharArray() const
{
    return this->charArray;
}

bool String::IsNull() const
{
    //Check if the char array is null
    return (this->charArray == nullptr);
}

String& String::operator=(const String& another)
{
    //If this string is the other string, do nothing
    if (this == &another)
    {
        //nothing
    }
    else
    {
        //First destroy this string
        this->~String();
        //If the other string is not null, copy it to this string
        if (!another.IsNull())
        {
            this->length = another.length;
            this->charArray = new char[this->length + 1];
            strcpy(this->charArray, another.charArray);
        }
    }
    //Return this string, so that we can do multiple assignment
    return *this;
}

bool String::operator==(const String& another) const
{
    //If either one of the strings is null, they are not equal
    if (this->IsNull() || another.IsNull())
    {
        return false;
    }
    //Otherwise compare their char arrays
    return (strcmp(this->charArray, another.charArray) == 0);
}

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

String& String::ReadNext(std::istream& stream, char separator, bool endOnNewLine)
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

    return *this;
}

String& String::ReadLine(std::istream& stream)
{
    //Read from stream until we reach a new line
    this->ReadNext(stream, '\n');

    return *this;
}

String& String::ReadWord(std::istream& stream)
{
    //Read from stream until we reach a space or a new line
    this->ReadNext(stream, ' ', true);

    return *this;
}

std::istream& operator>>(std::istream& stream, String& string)
{
    //Read a line from the stream
    string.ReadLine(stream);
    //Return the stream, so that we can do multiple operators
    return stream;
}

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

bool String::IsInt() const
{
    //If the string is null or empty, it is not parsable to int
    if (this->IsNull() || this->length == 0)
    {
        return false;
    }

    char* p = this->charArray;
    //If there is a sign in front, skip it, it's fine
    if (*p == '+' || *p == '-')
    {
        p++;
    }
    //There should be digits after that
    if (*p == '\0')
    {
        return false;
    }

    //All remaining characters should be digits
    for (; *p != '\0'; p++)
    {
        if (!IsDigit(*p))
        {
            return false;
        }
    }

    return true;
}

bool String::IsDouble() const
{
    //If the string is null or empty, it is not parsable to double
    if (this->IsNull() || this->length == 0)
    {
        return false;
    }

    unsigned pointsCount = this->Count('.');
    //There should be at most one decimal point in the number
    if (pointsCount > 1)
    {
        return false;
    }
    //If there is no decimal point, we check for int
    if (pointsCount == 0)
    {
        return this->IsInt();
    }

    //Find where digits begin by checking for a sign in front
    int digitsBegin = 0;
    if (this->charArray[0] == '+' || this->charArray[0] == '-')
    {
        digitsBegin++;
    }

    //All remaining characters should be digits and a point
    for (int i = digitsBegin; i < this->length; i++)
    {
        char c = this->charArray[i];
        //If the current character is a digit, it's fine
        if (IsDigit(c))
        {
            continue;
        }
        //Otherwise it should be the point and there should be digits on its both sides
        if (c != '.' || i <= digitsBegin || i >= this->length - 1)
        {
            return false;
        }
    }

    return true;
}

bool String::IsNumerical() const
{
    return (this->IsInt() || this->IsDouble());
}

int String::ParseToInt() const
{
    //If the string is not numerical, we cannot parse it
    if (!this->IsInt())
    {
        return 0;
    }
    //We'll traverse the string with a char pointer
    char* p = this->charArray;
    //Check for a sign in front
    bool sign = (*p == '+' || *p == '-');
    bool negative = (*p == '-');
    if (sign)
    {
        p++;
    }
    //Traverse the string and add digits one by one
    int result = 0;
    while (*p != '\0')
    {
        result = result * 10 + ParseToDigit(*p);
        p++;
    }
    //We traversed digits and got a positive number, but it could be negative
    return negative ? (-result) : result;
}

String ParseFromInt(int n)
{
    //If the number is negative, we'll parse it as positive and then flip the sign
    bool negative = (n < 0);
    if (negative)
    {
        n *= (-1);
    }
    //Find the length of the char array and create it
    int digitsCount = CountDigits(n);
    unsigned length = digitsCount + (negative ? 1 : 0);
    char* charArray = new char[length + 1];
    //Add digits one by one from the number to the char array
    int i = length - 1;
    do
    {
        charArray[i--] = ParseFromDigit(n % 10);
        n /= 10;
    } while (n != 0);
    //If the number was negative, add a minus sign in front
    if (negative)
    {
        charArray[0] = '-';
    }
    charArray[length] = '\0';
    //Create a string from the char array and free the memory
    String string(charArray);
    delete[] charArray;

    return string;
}

int ReadInt(std::istream& stream)
{
    //Read the number as a string
    String string;
    string.ReadNext(stream, ' ', true);
    //Parse it to int
    int result = string.ParseToInt();

    return result;
}

double String::ParseToDouble() const
{
    //If the string is not numerical, we cannot parse it
    if (!this->IsDouble())
    {
        return 0;
    }
    //Find the point
    int pointIndex = this->Find('.');
    //If there's no point, parse as int
    if (pointIndex == -1)
    {
        int n = this->ParseToInt();
        return (double)n;
    }
    //Find the whole part as a substring and parse it to int
    unsigned wholePartBegin = 0;
    unsigned wholePartLength = pointIndex;
    String wholePartString = this->GetSubstring(wholePartBegin, wholePartLength);
    int wholePart = wholePartString.ParseToInt();
    //Find the decimal part by adding digits one by one
    double decimalPart = 0;
    for (int i = this->length - 1; i > pointIndex; --i)
    {
        char c = this->charArray[i];
        decimalPart = decimalPart / 10 + (double)ParseToDigit(c) / 10;
    }
    //We got the decimal part digit by digit, so it's positive,
    //but it could be negative
    if (wholePart < 0)
    {
        return wholePart - decimalPart;
    }
    else
    {
        return wholePart + decimalPart;
    }
}

String ParseFromDouble(double d)
{
    //If the number is negative,
    //we'll parse it as positive and then flip the sign
    String sign = "";
    bool negative = (d < 0);
    if (negative)
    {
        d *= (-1);
        sign = "-";
    }
    //The whole part is just d casted to an int
    int wholePart = (int)d;
    //Find the decimalPart as an int digit by digit
    int decimalPart = 0;
    d -= wholePart;
    while (!CloseEnough(d, 0))
    {
        decimalPart *= 10;
        d *= 10;
        decimalPart += (int)d;
        d -= (int)d;
    }
    //Find the resulting string by parsing whole part and decimal part to int separately
    String string;
    if (decimalPart == 0)
    {
        string = sign + ParseFromInt(wholePart);
    }
    else
    {
        string = sign + ParseFromInt(wholePart) + "." + ParseFromInt(decimalPart);
    }

    return string;
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

String::~String()
{
    //Free the memory of the string's char array, if not null
    if (!this->IsNull())
    {
        delete[] this->charArray;
        //and make it null
        this->charArray = nullptr;
        this->length = 0;
    }
}