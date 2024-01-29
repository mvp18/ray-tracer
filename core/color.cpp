#include <core/color.h>
#include <core/scalar.h>
#include <core/homogeneouscoord.h>
#include <core/assert.h>

namespace rt {

RGBColor::RGBColor(const HomogeneousCoord& coord)
{
    this->r = coord.x;
    this->g = coord.y;
    this->b = coord.z;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    return RGBColor(this->r*c.r, this->g*c.g, this->b*c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
    bool bool_r = (fabs(this->r - c.r)<=epsilon);
    bool bool_g = (fabs(this->g - c.g)<=epsilon);
    bool bool_b = (fabs(this->b - c.b)<=epsilon);

    if (bool_r && bool_g && bool_b) return true;
    else return false;
}

bool RGBColor::operator != (const RGBColor& b) const {
    return !(*this==b);
}

RGBColor RGBColor::clamp() const {
    float r_c = this->r;
    float g_c = this->g;
    float b_c = this->b;

    if (r_c > 1) r_c = 1;
    if (r_c < 0) r_c = 0;

    if (g_c > 1) g_c = 1;
    if (g_c < 0) g_c = 0;

    if (b_c > 1) b_c = 1;
    if (b_c < 0) b_c = 0;

    return RGBColor(r_c, g_c, b_c);
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator * (const RGBColor& c, float scalar) {
    return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator / (const RGBColor& c, float scalar) {
    rt_assert(scalar!=0);
    return RGBColor(c.r/scalar, c.g/scalar, c.b/scalar);
}

}
