#include <rt/materials/material.h>
#include <rt/materials/combine.h>

namespace rt {

CombineMaterial::CombineMaterial()
{
    /* TODO */
}

void CombineMaterial::add(Material* m, float w) {
    MaterialList mat;
    mat.material = m;
    mat.weight = w;

    materials.push_back(mat);
    if (m->useSampling() == SAMPLING_NOT_NEEDED) SAMPLING_NOT_NEEDED_mat.push_back(mat);    
    if (m->useSampling() == SAMPLING_ALL) SAMPLING_ALL_mat.push_back(mat);
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const { 
    RGBColor result = RGBColor::rep(0.0f);
    for (auto& it : SAMPLING_NOT_NEEDED_mat){
        result = result + it.material->getReflectance(texPoint, normal, outDir, inDir) * it.weight;
    }

    return result;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor result = RGBColor::rep(0.0f);
    for (auto& it : materials) {
        result = result + it.material->getEmission(texPoint, normal, outDir) * it.weight;
    }
    return result;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    SampleReflectance SR;

    // pick one random material from SAMPLING_ALL_mat
    if (SAMPLING_ALL_mat.size() > 0) {
        int index = random() % SAMPLING_ALL_mat.size();
        SR = SAMPLING_ALL_mat[index].material->getSampleReflectance(texPoint, normal, outDir);
        SR.reflectance = SR.reflectance * SAMPLING_ALL_mat[index].weight;
    }
    return SR;
}

Material::Sampling CombineMaterial::useSampling() const {    
    if (SAMPLING_ALL_mat.size() == 0) return SAMPLING_NOT_NEEDED;
    if (SAMPLING_NOT_NEEDED_mat.size() == 0) return SAMPLING_ALL;
    else return SAMPLING_SECONDARY;
}

}