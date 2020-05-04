#pragma once

const double epsilon = 0.001;

//Returns the smaller of two objects
template <typename T>
T min(T a, T b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

//Returns the greater of two objects
template <typename T>
T max(T a, T b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

//Returns the absolute value of a numberical object
template <typename T>
T absVal(T x)
{
    if (x < 0)
    {
        return (-x);
    }
    return x;
}

//Checks if a char is a digit
bool IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

//Parses a char to a digit
int ParseToDigit(char c)
{
    return c - '0';
}

//Parses a digit to a char
char ParseFromDigit(int d)
{
    return d + '0';
}

//Counts the number of digits of a given number
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

//Checks if two numbers are close enough to each other
bool CloseEnough(double a, double b)
{
    return ( absVal(a - b) <= epsilon );
}