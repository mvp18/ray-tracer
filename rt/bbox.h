#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <cmath>
#include <vector>

using namespace std;

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;
    bool isEmpty, isFull;

    BBox() {}
    BBox(const Point& min, const Point& max) : min(min), max(max)
    {
        isEmpty = false;
        isFull = false;
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return (max - min);
    }

    float area() const {
        Vector d = diagonal();
        float l, w, h;
        l = fabs(d.x);
        w = fabs(d.y);
        h = fabs(d.z);
        return 2*(l*w + l*h + w*h);
    }

    std::pair<float, float> intersect(const Ray& ray) const;

    bool isUnbound() const;
    Point getCentroid() const;
    vector<Point> getCorners() const;
    vector<std::pair<float, int> > getSortedAxes() const;
};

}

#endif