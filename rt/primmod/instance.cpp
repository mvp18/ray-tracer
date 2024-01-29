#include <rt/primmod/instance.h>
#include <cmath>

namespace rt {

Instance::Instance(Primitive* content)
{
    reset();
    archetype = content;
    bbox = archetype->getBounds();
}

Primitive* Instance::content() {
    return archetype;
}

void Instance::reset() {    
    transform_ = Matrix::identity();
    inv_transform_ = Matrix::identity();
}

void Instance::ComposeTransforms(const Matrix& M){

    transform_ = product(M, transform_);
    inv_transform_ = transform_.invert();
}

void Instance::translate(const Vector& t) {
    
    Matrix T = Matrix::identity();
    T[0][3] = t.x;
    T[1][3] = t.y;
    T[2][3] = t.z;

    ComposeTransforms(T);
}

void Instance::rotate(const Vector& nnaxis, float angle) {
    
    Vector r = nnaxis.normalize();
    Vector s, s_, t;

    if (fabs(r.x)<fabs(r.y) && fabs(r.x)<fabs(r.z)) s_ = Vector(0, -r.z, r.y);
    if (fabs(r.y)<fabs(r.x) && fabs(r.y)<fabs(r.z)) s_ = Vector(-r.z, 0, r.x);
    if (fabs(r.z)<fabs(r.x) && fabs(r.z)<fabs(r.y)) s_ = Vector(-r.y, r.x, 0);
    s = s_.normalize();
    t = cross(r, s).normalize();

    Matrix M = Matrix::system(r, s, t);
    Matrix R_x = Matrix::identity();

    R_x[1][1] = cos(angle);
    R_x[1][2] = -sin(angle);
    R_x[2][1] = sin(angle);
    R_x[2][2] = cos(angle);

    Matrix R = product(product(M, R_x), M.transpose());

    ComposeTransforms(R);
}

void Instance::scale(float f) {
    scale(Vector(f, f, f));
}

void Instance::scale(const Vector& s) {
    
    Matrix S = Matrix::identity();
    S[0][0] = s.x;
    S[1][1] = s.y;
    S[2][2] = s.z;

    ComposeTransforms(S);
}

void Instance::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float tmin, float tmax) const {

    Point newOrigin = inv_transform_ * ray.o;
    Vector newDir = inv_transform_ * ray.d;
    float dir_length = newDir.length();
    
    Ray newRay = Ray(newOrigin, newDir.normalize());
    Intersection int_fail = Intersection::failure();

    Intersection hit = archetype->intersect(newRay, tmin * dir_length, tmax * dir_length);

    if (hit){
        Matrix inv_transpose = inv_transform_.transpose();
        inv_transpose[3] = HomogeneousCoord::rep(0.0f);
        Vector newNormal = inv_transpose * hit.normal();
        float newDist = hit.distance/dir_length;
        return Intersection(newDist, ray, hit.solid, newNormal.normalize(), hit.local());
    }
    else return int_fail;
}

BBox Instance::getBounds() const {
    
    vector<Point> old_corners = bbox.getCorners();
    BBox new_bbox = BBox::empty();

    for (auto& it : old_corners){
        new_bbox.extend(transform_ * it);
    }

    return new_bbox;
}

}