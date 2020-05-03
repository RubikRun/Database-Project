#pragma once

#define Debug(message) std::cout << "Debug console: " << message << std::endl;

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

template <typename T>
T absVal(T x)
{
    if (x < 0)
    {
        return (-x);
    }
    return x;
}

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

bool CloseEnough(double a, double b)
{
    return ( absVal(a - b) <= epsilon );
}