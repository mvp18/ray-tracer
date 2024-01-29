#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/assert.h>

namespace rt {

Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const HomogeneousCoord& coord)
{
    rt_assert(fabs(coord.w) <= rt::epsilon); // would fail for the transformed normal in instancing.cpp
    this->x = coord.x;
    this->y = coord.y;
    this->z = coord.z;
}

Vector Vector::operator + (const Vector& b) const {
    return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector Vector::operator - (const Vector& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

Vector Vector::operator - () const {
    return (*this)*(-1);
}

Vector Vector::normalize() const {
    return (*this)/length();
}

Vector operator * (float scalar, const Vector& b) {
    return Vector(scalar*b.x, scalar*b.y, scalar*b.z);
}

Vector operator * (const Vector& a, float scalar) {
    return Vector(scalar*a.x, scalar*a.y, scalar*a.z);
}

Vector operator / (const Vector& a, float scalar) {
    rt_assert(scalar!=0);
    return Vector(a.x/scalar, a.y/scalar, a.z/scalar);
}

Vector cross(const Vector& a, const Vector& b) {
    return Vector(a.y*b.z - b.y*a.z, a.z*b.x-a.x*b.z, a.x*b.y - a.y*b.x);
}

float dot(const Vector& a, const Vector& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Vector::lensqr() const {
    return dot(*this, *this);
}

float Vector::length() const {
    return sqrt(lensqr());
}


bool Vector::operator == (const Vector& b) const {
    bool bool_x = (fabs(this->x - b.x)<=epsilon);
    bool bool_y = (fabs(this->y - b.y)<=epsilon);
    bool bool_z = (fabs(this->z - b.z)<=epsilon);

    if (bool_x && bool_y && bool_z) return true;
    else return false;
}

bool Vector::operator != (const Vector& b) const {
    return !(*this==b);
}

Vector min(const Vector& a, const Vector& b) {
    return Vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Vector max(const Vector& a, const Vector& b) {
    return Vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

Point operator + (const Point& a, const Vector& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator + (const Vector& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator - (const Point& a, const Vector& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator * (const HomogeneousCoord& scale, const Point& p) {    
    return Point(p.x*scale.x/scale.w, p.y*scale.y/scale.w, p.z*scale.z/scale.w);
}

}
