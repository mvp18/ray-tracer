#include <rt/solids/quad.h>
#include <core/random.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    v1 = origin;
    span1_ = span1;
    span2_ = span2;
    
    v2 = v1 + span1_;
    v3 = v1 + span1_ + span2_;
    v4 = v1 + span2_;
    center = v1 + 0.5*span1_ + 0.5*span2_;

    normal = cross(span1_, span2_);

    min_ex = min(min(v1, v2), min(v3, v4));
    max_ex = max(max(v1, v2), max(v3, v4));
}

BBox Quad::getBounds() const {
    return BBox(min_ex, max_ex);
}

Intersection Quad::intersect(const Ray& ray, float tmin, float tmax) const {
    
    Intersection int_fail = Intersection::failure();

    /*https://graphics.cs.kuleuven.be/publications/LD05ERQIT/LD05ERQIT_paper.pdf*/

    float t, u, v;
    float alpha, beta, alpha_, beta_, alpha11, beta11;

    // reject rays using the barycentric coordinates of hit point wrt T1
    Vector pvec = cross(ray.d, span2_);
    float det = dot(pvec, span1_);

    // ray and quad are parallel if det is close to 0
    if (fabs(det) < rt::epsilon) return int_fail;

    float invDet = 1/det;
    Vector tvec = ray.o - v1;
    alpha = dot(tvec, pvec)*invDet;
    if (alpha < 0 || alpha > 1) return int_fail;

    Vector qvec = cross(tvec, span1_);
    beta = dot(ray.d, qvec)*invDet;
    if (beta < 0 || beta > 1) return int_fail;

    // reject rays using the barycentric coordinates of hit point wrt T2
    if (alpha + beta > 1){
        Vector pvec_ = cross(ray.d, -span2_);
        float det_ = dot(pvec_, -span1_);
        if (fabs(det_) < rt::epsilon) return int_fail;

        float invDet_ = 1/det_;
        Vector tvec_ = ray.o - v3;
        alpha_ = dot(tvec_, pvec_)*invDet_;
        if (alpha_ < 0) return int_fail;

        Vector qvec_ = cross(tvec_, -span1_);
        beta_ = dot(ray.d, qvec_)*invDet_;
        if (beta_ < 0) return int_fail;
    }

    t = dot(span2_, qvec)*invDet;
    if (t < tmin || t > tmax) return int_fail;

    // Compute barycentric coordinates of v3
    Vector v3v1 = v3 - v1;
    if ((fabs(normal.x) >= fabs(normal.y)) and (fabs(normal.x) >= fabs(normal.z))){
        alpha11 = (v3v1.y*span2_.z - v3v1.z*span2_.y)/normal.x;
        beta11 = (span1_.y*v3v1.z - span1_.z*v3v1.y)/normal.x;
    }
    else if ((fabs(normal.y) >= fabs(normal.x)) and (fabs(normal.y) >= fabs(normal.z))){
        alpha11 = (v3v1.z*span2_.x - v3v1.x*span2_.z)/normal.y;
        beta11 = (span1_.z*v3v1.x - span1_.x*v3v1.z)/normal.y;
    }
    else{
        alpha11 = (v3v1.x*span2_.y - v3v1.y*span2_.x)/normal.z;
        beta11 = (span1_.x*v3v1.y - span1_.y*v3v1.x)/normal.z;
    }

    //Compute bilinear coordinates of intersection point
    if (fabs(alpha11 - 1) < rt::epsilon){
        u = alpha;
        if (fabs(beta11 - 1) < rt::epsilon) v = beta;
        else v = beta/(u*(beta11 - 1) + 1);
    }
    else if (fabs(beta11 - 1) < rt::epsilon){
        v = beta;
        u = alpha/(v*(alpha11 - 1) + 1);
    }
    else{
        float A, B, sign_B, C, discriminant, q;

        A = -(beta11 - 1);
        B = alpha*(beta11 - 1) - beta*(alpha11 - 1) - 1;
        C = alpha;
        discriminant = B*B - 4*A*C;

        sign_B = (B > 0.0f) - (B < 0.0f);
        q = -0.5*(B + sign_B*sqrtf(discriminant));

        u = q/A;
        if (u < 0 || u > 1) u = C/q;
        v = beta/(u*(beta11 - 1) + 1);
    }

    return Intersection(t, ray, this, normal.normalize(), Point(u, v, 0));


    /*Plane-based Method*/
    // float den = dot(ray.d.normalize(), normal.normalize());

    // if (den==0) return int_fail;

    // float t = dot((center - ray.o), normal.normalize())/den;
    // if (t < tmin || t > tmax) return int_fail;

    // Point local_ = ray.getPoint(t);

    // bool check1 = dot(cross(v2 - v1, local_ - v1), normal.normalize()) >= 0;
    // bool check2 = dot(cross(v3 - v2, local_ - v2), normal.normalize()) >= 0;
    // bool check3 = dot(cross(v4 - v3, local_ - v3), normal.normalize()) >= 0;
    // bool check4 = dot(cross(v1 - v4, local_ - v4), normal.normalize()) >= 0;

    // if (check1 & check2 & check3 & check4) return Intersection(t, ray, this, normal.normalize(), local_);
    // else return int_fail;
}

Solid::Sample Quad::sample() const {
    
    Solid::Sample s;
    
    s.normal = normal.normalize();
    
    float u = random(0.0f, 1.0f);
    float v = random(0.0f, 1.0f);
    s.point = v1 + u*span1_ + v*span2_;

    return s;
}

float Quad::getArea() const {
    return normal.length();
}

}
