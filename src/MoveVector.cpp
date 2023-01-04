#include "MoveVector.h"
#include "cmath"

//template <typename T>
float MoveVector::length()
{
    return sqrt((this->x * this->x) + (this->y * this->y));
}

//template <typename T>
MoveVector MoveVector::normalize()
{
    float length = this->length();
    if (length != 0)
    {
        this->x = this->x / length;
        this->y = this->y / length;
    }

    return *this;
}