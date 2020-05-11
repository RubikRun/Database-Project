#pragma once

//Checks if a char is a digit
bool IsDigit(char c);

//Parses a char to a digit
int ParseToDigit(char c);

//Parses a digit to a char
char ParseFromDigit(int d);

//Counts the number of digits of a given number
int CountDigits(int n);

//Returns the absolute value of a numberical object
template <typename T>
T absVal(T x);

//Checks if two numbers are close enough to each other
bool CloseEnough(double a, double b);