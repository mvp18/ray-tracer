#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const HomogeneousCoord& coord)
{
    rt_assert(fabs(coord.w) > rt::epsilon); // can be 0 only for vanishing points / points at infinity
    this->x = coord.x/coord.w;
    this->y = coord.y/coord.w;
    this->z = coord.z/coord.w;
}

Vector Point::operator - (const Point& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

bool Point::operator == (const Point& b) const {
    
    bool bool_x = (fabs(this->x - b.x)<=epsilon);
    bool bool_y = (fabs(this->y - b.y)<=epsilon);
    bool bool_z = (fabs(this->z - b.z)<=epsilon);

    if (bool_x && bool_y && bool_z) return true;
    else return false;
}

bool Point::operator != (const Point& b) const {
    return !(*this==b);
}

Point operator * (float scalar, const Point& b) {
    return Point(scalar*b.x, scalar*b.y, scalar*b.z);
}

Point operator * (const Point& a, float scalar) {
    return Point(scalar*a.x, scalar*a.y, scalar*a.z);
}

Point min(const Point& a, const Point& b) {
    return Point(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Point max(const Point& a, const Point& b) {
    return Point(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

}
