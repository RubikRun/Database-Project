#pragma once

#include "String.hpp"

bool String::IsNumerical()
{
    //If the string is null or empty, it is not numerical
    if (this->IsNull() || this->length == 0)
    {
        return false;
    }
    //Check for a sign in front and find where digits begin
    int digitsBegin = 0;
    if (this->charArray[0] == '+' || this->charArray[0] == '-')
    {
        digitsBegin = 1;
    }
    //There can be at most 1 point in a number
    if (this->Count('.') > 1)
    {
        return false;
    }
    //Traverse digits (and point)
    for (int i = digitsBegin; i < this->length; ++i)
    {
        char c = this->charArray[i];
        //If a digit, it's fine
        if (IsDigit(c))
        {
            continue;
        }
        //Otherwise, it should be a point
        if (c != '.')
        {
            return false;
        }
        //and it should have digits on both sides,
        //so exclusively between first and last digit
        if (i <= digitsBegin || i >= this->length - 1)
        {
            return false;
        }
    }
    //At this point everything is checked
    return true;
}

int String::ParseToInt()
{
    //If the string is not numerical, we cannot parse it
    if (!this->IsNumerical())
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
    //We traversed digits and got a positive number,
    //but it could be negative
    return negative ? (-result) : result;
}

String ParseFromInt(int n)
{
    //If the number is negative,
    //we'll parse it as positive and then flip the sign
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

double String::ParseToDouble()
{
    //If the string is not numberical, we cannot parse it
    if (!this->IsNumerical())
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
    //Check for negative and find the sign
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