#include "BasicFunctions.hpp"
#include "Constants.hpp"

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