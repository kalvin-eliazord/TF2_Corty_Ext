#include "Vector3.h"

Vector3 Vector3::operator+(const Vector3& v2)
{
    return { this->x + v2.x, this->y + v2.y, this->z + v2.z };
}

Vector3 Vector3::operator-(const Vector3& v2)
{
    return { this->x - v2.x, this->y - v2.y, this->z - v2.z };
}

float Vector3::operator*(const Vector3& v2)
{
    return { (this->x * v2.x) + (this->y * v2.y) + (this->z * v2.z) };
}

bool Vector3::operator<(const Vector3& v2)
{
    if(this->x < v2.x && this->y < v2.y)
        return true;
    
    return false;
}