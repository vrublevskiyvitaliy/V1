//
//  Points.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef Points_hpp
#define Points_hpp

#include <stdio.h>

#include <glm/glm.hpp>

#include "RandomClass.hpp"

class Points{
public:
    const static int MAX_POINTS_NUMBER = 1000;

private:
   
    const static int DEFAULT_POINTS_NUMBER = 100;
    const static int DEFAULT_CHANGE_MOVE_ITERATIONS = 1000;
    
    const float upperBoundArea = 1;
    const float lowerBoundArea = -1;
    int numberOfApliedMoves = 0;
    
    int numberOfPoints;
    Random random;
    
    glm::vec3 pointsPositions[MAX_POINTS_NUMBER];
    glm::vec3 pointsMoves[MAX_POINTS_NUMBER];
    glm::vec3 pointsColors[MAX_POINTS_NUMBER];
public:
    
    Points(int number);
    void applyMove();
    void updateMoves();
    glm::vec3 * getPointsPositions();
    glm::vec3 * getPointsMoves();
    glm::vec3 * getPointsColors();
private:
    void updateIfOutOfArea();
    void initPoints();
};

#endif /* Points_hpp */
