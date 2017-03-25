//
//  test_kd_tree.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/22/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include <stdio.h>

#include "KDTree.hpp"

#include <iostream>

using namespace std;

#include "QuickSort.hpp"

int __main()
{
    QuickSort::testQuickSort(5, 0);
 /*
    KDTree tree(4);
    tree.setData();
    
    int n = 4;
    
    std::vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        points[i].x = (i + 1) * 0.3 - 1;
        points[i].y = (i + 1) * 0.3 - 1;
    }
    */
    /*
     tree[0]  null
     tree[1]           dim=0  [P,L,R]=[0,2,3]  pnt=[-0.10,-0.10]
     tree[2]           dim=1  [P,L,R]=[1,4,5]  pnt=[-0.40,-0.40]
     tree[3]           dim=1  [P,L,R]=[1,6,7]  pnt=[0.20,0.20]
     tree[4]  -> LEAF  dim=0  [P,L,R]=[2,8,9]  pnt=[-0.70,-0.70]
     tree[5]  -> LEAF  dim=0  [P,L,R]=[2,10,11]  pnt=[-0.40,-0.40]
     tree[6]  -> LEAF  dim=0  [P,L,R]=[3,12,13]  pnt=[-0.10,-0.10]
     tree[7]  -> LEAF  dim=0  [P,L,R]=[3,14,15]  pnt=[0.20,0.20]
     
     */
    
//    tree.build(points, false);
//    
//    
//    for(int i = 1; i < 8; i++) {
//        KDTreeNode node = tree.kd_tree[i];
//        node.setRGBA();
//        
//        KDTreeNode convert_node = node.getNodeByRGBA(node.r, node.g, node.b, node.a);
//        
//        if (node.compare(convert_node)) {
//            printf("Node %d similare\n", i);
//        } else {
//            printf("Node %d NOT similare, ERROR\n", i);
//        }
//        
//    }
    
    return 0;
}
