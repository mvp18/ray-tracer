#ifndef CG1RAYTRACER_HOMOGENEOUSCOORD_HEADER
#define CG1RAYTRACER_HOMOGENEOUSCOORD_HEADER

#include <core/macros.h>
#include <core/assert.h>

namespace rt {

class Point;
class Vector;

class ALIGN(16) HomogeneousCoord {
public:
    HomogeneousCoord() {}
    HomogeneousCoord(float x, float y, float z, float w);
    explicit HomogeneousCoord(const Point& p);
    explicit HomogeneousCoord(const Vector& v);

    static HomogeneousCoord rep(float v) {
        return HomogeneousCoord(v, v, v, v);
    }

    float& operator [] (int idx);
    float  operator [] (int idx) const;

    HomogeneousCoord operator + (const HomogeneousCoord& b) const;
    HomogeneousCoord operator - (const HomogeneousCoord& b) const;
    HomogeneousCoord operator * (const HomogeneousCoord& b) const;
    HomogeneousCoord operator / (const HomogeneousCoord& b) const;

    HomogeneousCoord operator - () const;

    bool operator == (const HomogeneousCoord& b) const;
    bool operator != (const HomogeneousCoord& b) const;

    float x, y, z, w;
};

HomogeneousCoord operator * (float scalar, const HomogeneousCoord& b);
HomogeneousCoord operator * (const HomogeneousCoord& a, float scalar);
HomogeneousCoord operator / (const HomogeneousCoord& a, float scalar);
float dot(const HomogeneousCoord& a, const HomogeneousCoord& b);

HomogeneousCoord min(const HomogeneousCoord& a, const HomogeneousCoord& b);
HomogeneousCoord max(const HomogeneousCoord& a, const HomogeneousCoord& b);

}

#endif