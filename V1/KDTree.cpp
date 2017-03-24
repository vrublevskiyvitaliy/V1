//
//  KDTree.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "KDTree.hpp"

KDTree::KDTree(int n) {
    p = new Points(n);
}

void KDTree::setData(int n)
{
    //free_data();
    p->applyMove();
    int nn = p->getNumberOfPoints();
    glm::vec3 * pointsPositions = p->getPointsPositions();
    std::vector<Point> points(nn);
    for (int i = 0; i < nn; i++) {
        points[i].x = pointsPositions[i].x;
        points[i].y = pointsPositions[i].y;
    }
    
    build(points, false);
}

std::vector<float> KDTree::getData() {
    std::vector<float> data(num_nodes * 4);
    
    for (int i = 0; i < num_nodes; i++) {
        data[i*4] = kd_tree[i].r;
        data[i*4 + 1] = kd_tree[i].g;
        data[i*4 + 2] = kd_tree[i].b;
        data[i*4 + 3] = kd_tree[i].a;
    }
    
    return data;
}

int KDTree::get_p(int node_idx) {
    return node_idx >> 1;
}
int KDTree::get_l(int node_idx) {
    return node_idx << 1;
}
int KDTree::get_r(int node_idx) {
    return (node_idx << 1) + 1;
}


int KDTree::is_leaf(int node_val) {
    return (node_val & BIT_LEAF) >> 31;
}
int KDTree::get_dim(int node_val) {
    return (node_val & BIT_DIM) >> 15;
}
int KDTree::get_px(int node_val) {
    return (node_val & BIT_P_X) >> 16;
}
int KDTree::get_py(int node_val) {
    return (node_val & BIT_P_Y) >> 0;
}

void KDTree::free_data() {
    
    if (buffer) {
        delete[] buffer;
    }
    if (kd_tree) {
        delete[] kd_tree;
    }
    if (uint8Array) {
        delete[] uint8Array;
    }
    if (points) {
        delete[] points;
    }

}

KDTree::~KDTree() {
    free_data();
 }

void KDTree::initKdTree(unsigned long num_points) {
    int tmp_max_depth = ceil( log(num_points * 1.) / log(2.) ); // log2
    int tmp_num_nodes = 1 << (tmp_max_depth+1);
    
    if( tmp_num_nodes != this->num_nodes || this->kd_tree == NULL){
        this->max_depth  = tmp_max_depth;
        this->num_nodes  = tmp_num_nodes;
        //this->buffer     = new ArrayBuffer(this.num_nodes*4);
        this->kd_tree    = new KDTreeNode[num_nodes ];
        //this->uint8Array = new Uint8Array(this.buffer);
    }
    
    int max   = 2;
    int depth = 0;
    int dim   = depth % 2;
    for(int i = 0; i < num_nodes - 1; i++){
        if( i == max-1 ) {
            max <<= 1;
            depth++;
            dim = (depth % 2) << 15;
        }
        kd_tree[i+1].dim = dim ? 1 : 0; // setting  dim bits ... always the sam
    }
}

/*
 KdTree.prototype.build([new Point(CTX, 1, 1, 1, 0, 0), new Point(CTX, 2, 2, 2, 0, 0), new Point(CTX, 3, 3, 3, 0, 0), new Point(CTX, 4, 0, 0, 0, 0) ], true)
 
 
 
 KdTree.prototype.quick_sort = new Quicksort();
 
 
KdTree.prototype.quick_select = new QuickSelect();
 
 
 tree[1]           dim=0  [P,L,R]=[0,2,3]  pnt=[2.00,2.00]  code=1310740
 KdTree.js:242 tree[2]           dim=1  [P,L,R]=[1,4,5]  pnt=[1.00,1.00]  code=688138
 KdTree.js:242 tree[3]           dim=1  [P,L,R]=[1,6,7]  pnt=[3.00,3.00]  code=1998878
 KdTree.js:242 tree[4]  -> LEAF  dim=0  [P,L,R]=[2,8,9]  pnt=[0.00,0.00]  code=-2147483648
 KdTree.js:242 tree[5]  -> LEAF  dim=0  [P,L,R]=[2,10,11]  pnt=[1.00,1.00]  code=-2146828278
 KdTree.js:242 tree[6]  -> LEAF  dim=0  [P,L,R]=[3,12,13]  pnt=[2.00,2.00]  code=-2146172908
 KdTree.js:242 tree[7]  -> LEAF  dim=0  [P,L,R]=[3,14,15]  pnt=[3.00,3.00]  code=-2145517538
 
 */


void KDTree::printTree() {
    for(int i = 0; i < num_nodes; i++) {
        if(kd_tree[i].isEmpty){
            printf("tree[%d]  null\n", i);
            //std::cout << "tree[" << i << "]  null" << std::endl;
        } else {
            int leaf     = kd_tree[i].isLeaf;
            float x      = kd_tree[i].p.x;
            float y      = kd_tree[i].p.y;
            int dim      = kd_tree[i].dim;
            int P        = get_p(i);
            int L        = get_l(i);
            int R        = get_r(i);
            
            printf("tree[%d]  %s  dim=%d  [P,L,R]=[%d,%d,%d]  pnt=[%.2f,%.2f] \n",
                i, leaf ? "-> LEAF":"       ", dim, P,L,R, x, y);
        }
    }
}

void KDTree::build(std::vector<Point> v_points, bool is_debug) {
    
    //this.points = points.slice(0); // make sure original array doesn't get messed up
    
    initKdTree(v_points.size());
   
    //this.buildRecursiveFast(1, spoints, 0, number_points-1 );
    buildIterative(v_points);
    if(is_debug){
        printTree();
    }
}


bool sort_x (Point i, Point j) { return (i.x < j.x); }
bool sort_y (Point i, Point j) { return (i.y < j.y); }

void KDTree::buildIterative(std::vector<Point> v_points) {
    int ptr_T = 0; // tree pointer for compressed tree-nodes (integer)
    int ptr_P = 1; // stack pointer for point-sets
    
    std::vector< std::vector<Point> > stack_P(v_points.size() * v_points.size()); // FIFO
    stack_P[ptr_P++] = v_points;
    
    while(ptr_T++ < num_nodes) {
        
        std::vector<Point> p_v = stack_P[ptr_T];
        
        if(!p_v.size()) continue;
        
        unsigned long e = p_v.size();
        unsigned long m = e >> 1;
        
        if( e > 1 ){ // not a leaf
            
            //if( dim == 0) this.quicksortX(points, lo, hi);
            //if( dim == 1) this.quicksortY(points, lo, hi);
            
            if (kd_tree[ptr_T].dim == 0) {
                //sort x
                std::sort (p_v.begin(), p_v.end(), sort_x);
            } else {
                //sort y
                std::sort (p_v.begin(), p_v.end(), sort_y);
            }
            //this.quick_sort.sort(pnts, kd_tree[ptr_T].dim);
            
            //std::vector<Point> v_1(p_v.begin(), p_v.begin() + m);
            stack_P[ptr_P++] = std::vector<Point> (p_v.begin(), p_v.begin() + m);
            stack_P[ptr_P++] = std::vector<Point> (p_v.begin() + m, p_v.end());
        } else { // leaf
            ptr_P += 2;
            kd_tree[ptr_T].isLeaf = 1;
            kd_tree[ptr_T].isEmpty = false;
        }
        
        kd_tree[ptr_T].p.x = p_v[m].x;
        kd_tree[ptr_T].p.y = p_v[m].y;
        kd_tree[ptr_T].isEmpty = false;
        kd_tree[ptr_T].setRGBA();
    }
    
}

int KDTree::getNumNodes() {
    return num_nodes;
}





