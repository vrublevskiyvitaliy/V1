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
    // node encoding masks
    unsigned int  BIT_LEAF = 0x80000000; //    31
    unsigned int  BIT_SIGN_X  = 0x40000000; //    30
    unsigned int  BIT_DIM  = 0x00008000; //    15
    unsigned int  BIT_SIGN_Y  = 0x00004000; //    14
    unsigned int  BIT_P_Y  = 0x00003FFF; //  0-13
    unsigned int  BIT_P_X  = 0x3FFF0000; // 16-29
    
    int num_nodes = 0;
    int * buffer = NULL;     // ArrayBuffer
    unsigned int * uint8Array = NULL; // Uint8Array: view for texture-data
    
    int max_depth = 0;
    
    Point * points = NULL;    // copy of input points
    
    int kd_tree_planes_buffer = 0;
    int kd_tree_planes = 0;
    int BUF_IDX = 0;
    void free_data();
    Points * p = NULL;
public:
    
    KDTreeNode * kd_tree = NULL;    // Int32Array: view for saving values
    
    KDTree(int n = 10);
    ~KDTree();
    void setData(int n = 10);
    std::vector<float> getData();
    int getNumNodes();
    
    // get node indices for: Parent, Left-child, Right-child
    int get_p(int node_idx);
    int get_l(int node_idx);
    int get_r(int node_idx);

    // get node data: leaf, dimension, point.x, point.y
    int is_leaf(int node_val);
    int get_dim(int node_val);
    int get_px(int node_val);
    int get_py(int node_val);
    
    
    void initKdTree(unsigned long num_points);
    void printTree();
    
    void build(std::vector<Point> v_points, bool is_debug = true);
    void buildIterative(std::vector<Point> v_points);


};


#endif /* KDTree_hpp */
