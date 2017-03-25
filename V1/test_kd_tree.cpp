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
#include "QuickSelect.hpp"


int __main()
{
    //QuickSort::testQuickSort(5, 0);
    
    //QuickSelect q;
    //q.testSort(6, 0);
 
    int n = 4;
    std::vector<glm::vec2> points(n);
    
    for (int i = 0; i < n; i++) {
        points[i].x = (i + 1) * 0.3 - 1;
        points[i].y = (i + 1) * 0.3 - 1;
    }
    
    KDTree tree(n);
    
    tree.setData(points);
    
    return 0;
}
