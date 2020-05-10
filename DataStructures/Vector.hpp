#pragma once

#include <iostream>

template <typename T>
class Vector
{
    //The array where we keep the objects
    T* array;
    unsigned length, capacity;

    public:

        //Creates an empty vector
        Vector();

        //Creates a vector from another vector
        Vector(const Vector<T>& another);

        //Assigns another vector to this vector
        Vector<T>& operator=(const Vector<T>& another);

        //Returns the length of the vector
        unsigned GetLength() const;

        //Writes the elements of the vector to a stream, separated by a separator
        void Write(std::ostream& stream = std::cout, char separator = '\n') const;

        //Prints the elements of the vector
        void Print() const;

        //Returns a reference to the object at the given index
        T& operator[](unsigned index) const;

        //Adds an element to the vector
        void Add(T element);

        //Removes the element at the given index
        void RemoveAt(unsigned i);

        //Checks if the vector is empty
        bool IsEmpty() const;

        //Empties the vector
        void Empty();

    private:

        //Creates a new array with a given capacity and copies the elements to it
        void ChangeCapacity(unsigned newCapacity);

    public:

        //Frees the memory
        ~Vector();
};

template <typename T>
Vector<T>::Vector()
{
    //Create an empty vector
    this->array = nullptr;
    this->length = this->capacity = 0;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& another)
{
    //Create an empty vector
    this->array = nullptr;
    this->length = this->capacity = 0;

    //Assign the other vector to this vector
    *this = another;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& another)
{
    //If this vector is the other vector, do nothing
    if (this == &another)
    {
        return *this;
    }

    //Empty this vector
    this->Empty();
    //Copy the other vector to this vector
    this->length = another.length;
    this->capacity = another.capacity;
    this->array = new T[this->length];
    for (int i = 0; i < this->length; i++)
    {
        this->array[i] = another.array[i];
    }

    //Return this vector, so that we can do multiple assignment
    return *this;
}

template <typename T>
unsigned Vector<T>::GetLength() const
{
    //Return the length of the vector
    return this->length;
}

template <typename T>
void Vector<T>::Write(std::ostream& stream, char separator) const
{
    //Write each element to the stream
    for (int i = 0; i < this->length; i++)
    {
        stream << this->array[i];
        //After each element, except the last one, write the separator
        if (i < this->length - 1)
        {
            stream << separator;
        }
    }
}

template <typename T>
void Vector<T>::Print() const
{
    this->Write(std::cout, '\n');
    std::cout << "\n\n";
}

template <typename T>
T& Vector<T>::operator[](unsigned index) const
{
    //Return the element at the given index
    return this->array[index];
}

template <typename T>
void Vector<T>::Add(T element)
{
    //If the vector is empty, create a vector with only the element
    if (this->IsEmpty())
    {
        this->array = new T[1];
        this->array[0] = element;
        this->length = this->capacity = 1;
        return;
    }
    //If there is no capacity for the new element, double the capacity
    if (this->length == this->capacity)
    {
        this->ChangeCapacity(this->capacity * 2);
    }
    //Add the new element to the array
    this->array[this->length++] = element;
}

template <typename T>
void Vector<T>::RemoveAt(unsigned index)
{
    //If the index is outside the elements of the vector, do nothing
    if (index >= this->length)
    {
        return;
    }
    //If there is only one element in the vector, then the vector becomes empty
    if (this->length == 1)
    {
        this->~Vector<T>();
        return;
    }

    //Remove the element from the array
    for (int i = index; i < this->length - 1; i++)
    {
        this->array[i] = this->array[i + 1];
    }
    this->length--;

    //If the vector is half-full or less, we can cut the capacity in half
    if (this->length <= this->capacity / 2)
    {
        this->ChangeCapacity(this->capacity / 2);
    }
}

template <typename T>
bool Vector<T>::IsEmpty() const
{
    return (this->array == nullptr);
}

template <typename T>
void Vector<T>::Empty()
{
    if (this->array != nullptr)
    {
        delete[] this->array;
        this->array = nullptr;
    }
    this->length = this->capacity = 0;
}

template <typename T>
void Vector<T>::ChangeCapacity(unsigned newCapacity)
{
    //Copy the elements to a temporary array
    T* tempArr = new T[this->length];
    for (int i = 0; i < this->length; i++)
    {
        tempArr[i] = this->array[i];
    }

    //Free the memory of the vector's array
    delete[] this->array;

    //Create a new array with the given capacity
    //and copy back the elements from the temporary array
    this->capacity = newCapacity;
    this->array = new T[this->capacity];
    for (int i = 0; i < this->length; i++)
    {
        this->array[i] = tempArr[i];
    }

    //Free the memory of the temporary array
    delete[] tempArr;
}

template <typename T>
Vector<T>::~Vector()
{
    //Empty the vector
    this->Empty();
}