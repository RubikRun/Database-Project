#pragma once

#include "String.hpp"
#include "BasicFunctions.hpp"

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