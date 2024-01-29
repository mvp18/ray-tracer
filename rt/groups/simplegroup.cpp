#include <rt/groups/simplegroup.h>
using namespace std;

namespace rt {

BBox SimpleGroup::getBounds() const {
    return b;
}

Intersection SimpleGroup::intersect(const Ray& ray, float tmin, float tmax) const {

    Intersection int_final = Intersection::failure();
    Intersection curr_int;

    for (auto& it : primitives_){
        curr_int = it->intersect(ray, tmin, tmax);
        if (curr_int){
            tmax = curr_int.distance;
            int_final = curr_int;
        }
    }

    return int_final;
    
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    primitives_.push_back(p);
    b.extend(p->getBounds());
}

void SimpleGroup::setMaterial(Material* m) {    
    for (auto& it : primitives_) it->setMaterial(m);
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {    
    for (auto& it : primitives_) it->setCoordMapper(cm);
}

}
