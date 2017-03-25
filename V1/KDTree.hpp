//
//  KDTree.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef KDTree_hpp
#define KDTree_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#include "Points.hpp"
#include "KDTreeNode.hpp"
#include "QuickSort.hpp"
#include "QuickSelect.hpp"

#include <glm/glm.hpp>


class KDTree {
    int num_nodes = 0;
    int num_point = 0;
    
    int max_depth = 0;
    
    glm::vec2 * points = NULL;
    
    void free_data();
    Points * p = NULL;
    
    int build_algorithm = BUILD_ITERATIVE;
public:
    
    static const int BUILD_ITERATIVE = 0;
    static const int BUILD_RECURSIVE = 1;
    static const int BUILD_RECURSIVE_FAST = 2;

    KDTreeNode * kd_tree = NULL;    // Int32Array: view for saving values
    
    KDTree(int n = 4, int _build_algorithm = BUILD_ITERATIVE);
    ~KDTree();
    void setData();
    void setData(std::vector<glm::vec2> points);
    
    std::vector<float> getData();
    int getNumNodes();
    
    // get node indices for: Parent, Left-child, Right-child
    int get_p(int node_idx);
    int get_l(int node_idx);
    int get_r(int node_idx);
    
    void initKdTree(unsigned long num_points);
    void printTree();
    
    void build(std::vector<glm::vec2> v_points, bool is_debug = true);
    
    void buildIterative(std::vector<glm::vec2> v_points);
    void buildRecursive(int idx, std::vector<glm::vec2> points);
    
};


#endif /* KDTree_hpp */
