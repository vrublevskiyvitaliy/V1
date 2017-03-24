//
//  Points.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "Points.hpp"
/*
class Points{
    int numberOfPoints;
    const static int MAX_POINTS_NUMBER = 1000;
    glm::vec3 pointsPositions[MAX_POINTS_NUMBER];
    glm::vec3 pointsMoves[MAX_POINTS_NUMBER];
    glm::vec3 pointsColors[MAX_POINTS_NUMBER];
public:
  */
Points::Points(int number) {
    numberOfPoints = number > 0 && number < MAX_POINTS_NUMBER
        ? number
        : DEFAULT_POINTS_NUMBER;
    initPoints();
}

void Points::initPoints() {
    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointsPositions[i].x = random.getRandomPosition();
        pointsPositions[i].y = random.getRandomPosition();
        pointsPositions[i].z = 0;
        
        pointsMoves[i].x = random.getRandomMove();
        pointsMoves[i].y = random.getRandomMove();
        pointsMoves[i].z = 0;
        
        pointsColors[i].x = random.getRandom();
        pointsColors[i].y = random.getRandom();
        pointsColors[i].z = random.getRandom();
    }
}

int Points::getNumberOfPoints() {
    return numberOfPoints;
}

void Points::applyMove() {
    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointsPositions[i] += pointsMoves[i];
    }
    updateIfOutOfArea();
    
    numberOfApliedMoves = (numberOfApliedMoves + 1) % DEFAULT_CHANGE_MOVE_ITERATIONS;
    
    if (!numberOfApliedMoves) {
        updateMoves();
    }
}
void Points::updateMoves() {
    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointsMoves[i].x = random.getRandomMove();
        pointsMoves[i].y = random.getRandomMove();
    }
}
glm::vec3 * Points::getPointsPositions() {
    return pointsPositions;
}

glm::vec3 * Points::getPointsMoves() {
    return pointsMoves;
}
glm::vec3 * Points::getPointsColors() {
    return pointsColors;
}

void Points::updateIfOutOfArea() {
    float diff = upperBoundArea - lowerBoundArea;
    
    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointsPositions[i].x = pointsPositions[i].x > upperBoundArea
            ? pointsPositions[i].x - diff
            : pointsPositions[i].x;
        pointsPositions[i].y = pointsPositions[i].y > upperBoundArea
            ? pointsPositions[i].y - diff
            : pointsPositions[i].y;
        pointsPositions[i].x = pointsPositions[i].x < lowerBoundArea
            ? pointsPositions[i].x + diff
            : pointsPositions[i].x;
        pointsPositions[i].y = pointsPositions[i].y < lowerBoundArea
            ? pointsPositions[i].y + diff
            : pointsPositions[i].y;
    }
}
