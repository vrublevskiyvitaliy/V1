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
#include "Point.hpp"
#include "KDTreeNode.hpp"


class KDTree {
    int num_nodes = 0;
    int num_point = 0;
    
    int max_depth = 0;
    
    Point * points = NULL;
    
    void free_data();
    Points * p = NULL;
public:
    
    KDTreeNode * kd_tree = NULL;    // Int32Array: view for saving values
    
    KDTree(int n = 4);
    ~KDTree();
    void setData();
    std::vector<float> getData();
    int getNumNodes();
    
    // get node indices for: Parent, Left-child, Right-child
    int get_p(int node_idx);
    int get_l(int node_idx);
    int get_r(int node_idx);
    
    void initKdTree(unsigned long num_points);
    void printTree();
    
    void build(std::vector<Point> v_points, bool is_debug = true);
    void buildIterative(std::vector<Point> v_points);


};


#endif /* KDTree_hpp */
