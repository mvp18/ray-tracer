#include <rt/groups/bvh.h>
#include <stack>
#include <vector>

#include <omp.h>

using namespace std;

namespace rt {

BVH::BVH()
{
    root = new BVHNode(); // should I be using malloc instead?
}

void BVH::rebuildIndex() {
    root->prim_ids = prim_indices;
    root->nprim = prim_indices.size();
    root->bbox = getBounds();
    root->area = root->bbox.area();
    buildBVH(root);
}

float BVH::findBBcentroid(BVHNode* node, int axis){
    return getElementbyAxis(node->bbox.getCentroid(), axis);
}

float BVH::findBBExtent(BVHNode* node, int axis){
    return getElementbyAxis(node->bbox.max, axis) - getElementbyAxis(node->bbox.min, axis);
}

float BVH::getElementbyAxis(const Point p, int axis){
    if (axis==0) return p.x;
    if (axis==1) return p.y;
    else return p.z;
}

typedef struct bucket {
    
    int num_primitives = 0;
    BBox bbox = BBox::empty();

} BucketInfo;

float BVH::SAHsplit(BVHNode* node, int axis){

    // https://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies#fragment-CreateleafmonoBVHBuildNode-0

    BucketInfo buckets[12];
    float AxisExtent = findBBExtent(node, axis);

    BBox prim_bbox;
    float prim_centroid;
    int b;

    for (auto& it : node->prim_ids){
        prim_bbox = bvh_primitives[it]->getBounds();
        prim_centroid = getElementbyAxis(prim_bbox.getCentroid(), axis);
        b = int((prim_centroid - getElementbyAxis(node->bbox.min, axis))*(nbuckets-1)/AxisExtent);

        if (b==nbuckets) b = nbuckets - 1;

        buckets[b].num_primitives++;
        buckets[b].bbox.extend(prim_bbox);
    }

    float cost;
    float minCost = FLT_MAX;
    int splitbin;
    
    for (int i=0; i<nbuckets; ++i){
        
        BBox b_left=BBox::empty(), b_right=BBox::empty();
        int c_left=0, c_right=0;
        
        for (int j=0; j<=i; ++j){
            b_left.extend(buckets[j].bbox);
            c_left += buckets[j].num_primitives;
        }

        for (int j=i+1; j<nbuckets; ++j){
            b_right.extend(buckets[j].bbox);
            c_right += buckets[j].num_primitives;
        }
        cost = 0.125 + (c_left*b_left.area() + c_right*b_right.area())/node->area;
        if (cost < minCost){
            minCost = cost;
            splitbin = i;
        }
    }

    return getElementbyAxis(node->bbox.min, axis) + AxisExtent*float(splitbin)/float(nbuckets);
}

void BVH::fill_children(BVHNode* node, int size, int axis){

    float AxisSplitL;

    if (size>4 && useSAH_) AxisSplitL = SAHsplit(node, axis);
    else AxisSplitL = findBBcentroid(node, axis);
    
    for (auto& it : node->prim_ids){
        if (getElementbyAxis(bvh_primitives[it]->getBounds().getCentroid(), axis) <= AxisSplitL)
            node->left->add_prim_idx(bvh_primitives[it], it);
        else node->right->add_prim_idx(bvh_primitives[it], it);
    }
}

void BVH::buildBVH(BVHNode* node){
    
    vector<std::pair<float, int> > sorted_axes;

    if (node->nprim==0) return;
    if (node->nprim<=3){
        node->isLeaf = true;
        return;
    }

    if (node->nprim>3){
        node->left = new BVHNode();
        node->right = new BVHNode();

        sorted_axes = node->bbox.getSortedAxes();

        // try to split along largest axis
        fill_children(node, node->nprim, sorted_axes[2].second);

        // try to split along second largest axis
        if (node->left->nprim==0 || node->right->nprim==0){
            
            delete node->left;
            delete node->right;

            node->left = new BVHNode();
            node->right = new BVHNode();

            fill_children(node, node->nprim, sorted_axes[1].second);
        }

        // try to split along smallest axis
        if (node->left->nprim==0 || node->right->nprim==0){
            delete node->left;
            delete node->right;

            node->left = new BVHNode();
            node->right = new BVHNode();

            fill_children(node, node->nprim, sorted_axes[0].second);
        }

        // nothing works, so random split
        if (node->left->nprim==0 || node->right->nprim==0){
            delete node->left;
            delete node->right;

            node->left = new BVHNode();
            node->right = new BVHNode();

            for (int i=0; i<node->nprim; ++i){
                if (i<node->nprim/2){
                    node->left->add_prim_idx(bvh_primitives[node->prim_ids[i]], node->prim_ids[i]);
                    continue;
                }
                node->right->add_prim_idx(bvh_primitives[node->prim_ids[i]], node->prim_ids[i]);
            }
        }

        rt_assert(node->left->nprim!=0 && node->right->nprim!=0);

        // some weird hack: bbox bounds and areas won't be accurate.

        // if (node->left->prim_ids.size()==0) {
        //     node->left->add_prim_idx(bvh_primitives[node->right->prim_ids[0]], node->right->prim_ids[0]);
        //     node->right->prim_ids.erase(node->right->prim_ids.begin());
        // }

        // if (node->right->prim_ids.size()==0) {
        //     node->right->add_prim_idx(bvh_primitives[node->left->prim_ids[0]], node->left->prim_ids[0]);
        //     node->left->prim_ids.erase(node->left->prim_ids.begin());
        // }

        buildBVH(node->left);
        buildBVH(node->right);
    }
}

BBox BVH::getBounds() const {
    
    BBox bvh_bbox = BBox::empty();
    for (auto& it : bvh_primitives) bvh_bbox.extend(it->getBounds());

    return bvh_bbox;
}

Intersection BVH::intersect(const Ray& ray, float tmin, float tmax) const {
    
    BVHNode* node;
    BVHNode *left, *right, *first_child, *second_child;
    stack<BVHNode *> nodes;

    Intersection int_final = Intersection::failure();
    Intersection curr_int;

    nodes.push(root);

    #pragma omp parallel
    while (!nodes.empty()){
        node = nodes.top();
        nodes.pop();

        if (node->isLeaf){
            for (auto& it : node->prim_ids){
                curr_int = bvh_primitives[it]->intersect(ray, tmin, tmax);
                if (curr_int){
                    tmax = curr_int.distance;
                    int_final = curr_int;
                }
            }
        }
        else{
            left = node->left;
            right = node->right;

            auto tleft = left->bbox.intersect(ray);
            auto tright = right->bbox.intersect(ray);

            first_child = (tleft.first <= tright.first) ? left : right;
            second_child = (tleft.first <= tright.first) ? right : left;

            auto t1 = (tleft.first <= tright.first) ? tleft : tright;
            auto t2 = (tleft.first <= tright.first) ? tright : tleft;

            if (t1.first <= t1.second) nodes.push(first_child);
            if (t2.first <= t2.second) nodes.push(second_child);
        }
    }

    return int_final;
}

void BVH::add(Primitive* p) {
    bvh_primitives.push_back(p);
    prim_indices.push_back(nprimitives);
    nprimitives++;
}

void BVH::setMaterial(Material* m) {    
    for (auto& it : bvh_primitives) it->setMaterial(m);
}

void BVH::setCoordMapper(CoordMapper* cm) {
    for (auto& it : bvh_primitives) it->setCoordMapper(cm);
}

void BVH::serialize(BVH::Output& output) {
    // To implement this function:
    // - Call output.setNodeCount() with the number of nodes in the BVH
    /* TODO */
    // - Set the root node index using output.setRootId()
    /* TODO */
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
    /* TODO */ NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
    /* TODO */ NOT_IMPLEMENTED;
}

}