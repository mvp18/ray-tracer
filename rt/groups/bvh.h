#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <rt/primitive.h>

using namespace std;

namespace rt {

class BVHNode{
public:
    BBox bbox;
    BVHNode* left;
    BVHNode* right;
    vector<int> prim_ids;
    int nprim;
    bool isLeaf;
    float area;

    BVHNode(){
        bbox = BBox::empty();
        left = nullptr;
        right = nullptr;
        isLeaf = false;
        nprim = 0;
        area = 0;
    }

    void add_prim_idx(Primitive* p, int idx){
        prim_ids.push_back(idx);
        bbox.extend(p->getBounds());
        nprim++;
        area = bbox.area();
    }
};


class BVH : public Group {
public:
    BVH();

    BVHNode* root;
    vector<Primitive *> bvh_primitives;
    vector<int> prim_indices;
    int nprimitives = 0;
    bool useSAH_ = false;
    int nbuckets = 12;

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);

    virtual void fill_children(BVHNode* node, int size, int axis);
    virtual void buildBVH(BVHNode* node);
    virtual float findBBcentroid(BVHNode* node, int axis); // find midpoint of bbox of node along axis
    virtual float findBBExtent(BVHNode* node, int axis); // find extent of bbox along axis
    virtual float getElementbyAxis(const Point p, int axis);
    virtual float SAHsplit(BVHNode* node, int axis);

    // Do not use this structure as your node layout:
    // It is inefficient and has a large memory footprint.
    struct SerializedNode {
        bool isLeaf;
        BBox bbox;

        // For leaves only
        std::vector<Primitive*> primitives;

        // For internal nodes only
        size_t leftChildId;
        size_t rightChildId;
    };

    // Implement this function if you want to take part in the BVH speed competition
    struct Output {
        virtual void setNodeCount(size_t nodeCount) = 0;
        virtual void setRootId(size_t rootId) = 0;
        virtual void writeNode(size_t nodeId, const SerializedNode& node) = 0;
    };
    void serialize(Output& output);

    // Implementing this is optional and not needed for the BVH speed competition,
    // but it will allow you to run the benchmark utility and test your BVH exporter.
    struct Input {
        virtual size_t getNodeCount() = 0;
        virtual size_t getRootId() = 0;
        virtual const SerializedNode& readNode(size_t nodeId) = 0;
    };
    void deserialize(Input& input);
};

}

#endif