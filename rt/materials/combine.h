#ifndef CG1RAYTRACER_MATERIALS_COMBINE_HEADER
#define CG1RAYTRACER_MATERIALS_COMBINE_HEADER

#include <vector>

namespace rt {

class CombineMaterial : public Material {
public:
    CombineMaterial();
    typedef struct MaterialList {
        Material* material;
        float weight;
    } MaterialList;
    
    void add(Material* material, float weight);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;
private:
    std::vector<MaterialList> materials, SAMPLING_ALL_mat, SAMPLING_NOT_NEEDED_mat;
};

}

#endif