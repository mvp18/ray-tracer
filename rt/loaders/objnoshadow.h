#ifndef CG1RAYTRACER_LOADERS_OBJNOSHADOW_HEADER
#define CG1RAYTRACER_LOADERS_OBJNOSHADOW_HEADER

#include <string>
#include <map>
#include <vector>

namespace rt {

class Group;
class Material;

typedef std::map<std::string, Material*> MatLib;

void loadOBJ_noshadow(Group* dest, const std::string& path, const std::string& filename, MatLib* inmats=nullptr);
void loadOBJMat(MatLib* dest, const std::string& path, const std::string& filename);

}

#endif