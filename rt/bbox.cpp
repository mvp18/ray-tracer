#include <rt/bbox.h>
#include <rt/ray.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <cfloat>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

namespace rt {

BBox BBox::empty() {
    BBox b;
    b.min = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    b.max= Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    b.isEmpty = true;
    b.isFull = false;
    return b;
}

BBox BBox::full() {
    BBox b;
    b.min= Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    b.max = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    b.isFull = true;
    b.isEmpty = false;
    return b;
}

void BBox::extend(const Point& point) {
    
    if (isEmpty){
        min = point;
        max = point;
        isEmpty = false;
    }
    else{
        min = rt::min(min, point);
        max = rt::max(max, point);
    }
}

void BBox::extend(const BBox& bbox) {

    if (isEmpty){
        min = bbox.min;
        max = bbox.max;
        isEmpty = false;
    }
    else{
        min = rt::min(min, bbox.min);
        max = rt::max(max, bbox.max);
    }
}

Point BBox::getCentroid() const {

    return min + 0.5f * diagonal();
}

vector<Point> BBox::getCorners() const {

    vector<Point> corners;

    corners.push_back(min);
    corners.push_back(Point(min.x, min.y, max.z));
    corners.push_back(Point(min.x, max.y, min.z));
    corners.push_back(Point(min.x, max.y, max.z));
    corners.push_back(Point(max.x, min.y, min.z));
    corners.push_back(Point(max.x, min.y, max.z));
    corners.push_back(Point(max.x, max.y, min.z));
    corners.push_back(max);

    return corners;
}

vector<std::pair<float, int> > BBox::getSortedAxes() const {
    
    Vector diagonal = max - min;

    vector<std::pair<float, int> > sorted_ax;
    sorted_ax.push_back(make_pair(fabs(diagonal.x), 0));
    sorted_ax.push_back(make_pair(fabs(diagonal.y), 1));
    sorted_ax.push_back(make_pair(fabs(diagonal.z), 2));

    sort(sorted_ax.begin(), sorted_ax.end());

    return sorted_ax;
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {

    std::pair<float, float> int_fail(1.0f, 0.0f); // (t_near > t_far) hence no intersection

    if (isEmpty) return int_fail;

    Point bounds[2];
    bounds[0] = min;
    bounds[1] = max;
    
    int sign[3];
    sign[0] = (ray.inv_dir.x < 0);
    sign[1] = (ray.inv_dir.y < 0);
    sign[2] = (ray.inv_dir.z < 0);

    float t1, t2, tymin, tymax, tzmin, tzmax;

    t1 = (bounds[sign[0]].x - ray.o.x)*ray.inv_dir.x; //txmin
    t2 = (bounds[1-sign[0]].x - ray.o.x)*ray.inv_dir.x; // txmax

    tymin = (bounds[sign[1]].y - ray.o.y)*ray.inv_dir.y;
    tymax = (bounds[1-sign[1]].y - ray.o.y)*ray.inv_dir.y;

    if (t1 > tymax || tymin > t2) return int_fail;

    if (tymin > t1) t1 = tymin;
    if (tymax < t2) t2 = tymax;

    tzmin = (bounds[sign[2]].z - ray.o.z)*ray.inv_dir.z;
    tzmax = (bounds[1-sign[2]].z - ray.o.z)*ray.inv_dir.z;

    if (t1 > tzmax || tzmin > t2) return int_fail;

    if (tzmin > t1) t1 = tzmin;
    if (tzmax < t2) t2 = tzmax;

    return std::pair<float, float>(t1, t2);
}

bool BBox::isUnbound() const {
    return
        min.x==-FLT_MAX && max.x==FLT_MAX &&
        min.y==-FLT_MAX && max.y==FLT_MAX &&
        min.z==-FLT_MAX && max.z==FLT_MAX;
}

}