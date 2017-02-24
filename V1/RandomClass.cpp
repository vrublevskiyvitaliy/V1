//
//  RandomClass.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//
#include <time.h>
#include <stdlib.h>

#include "RandomClass.hpp"

Random::Random() {
    // make random seed
    time_t t;
    srand((unsigned) time(&t));
}

float Random::getRandom() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float Random::getRandomPosition() {
    return getRandomInRange(-1, 1);
}

float Random::getRandomMove() {
    return getRandomInRange(-0.002, 0.002);
}

float Random::getRandomInRange(float min, float max) {
    float r = getRandom();
    float dist = max - min;
    r *= dist;
    r += min;
    return r;
}
