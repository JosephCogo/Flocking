//
//  Boid.h
//  FlockingFinal
//
//  Created by Joseph Milsom on 5/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//

#ifndef __FlockingFinal__Boid__
#define __FlockingFinal__Boid__

#include <iostream>
#include "Vec3f.h"
#include <vector>

typedef struct boidStruct{
    Vec3f *pos;
    Vec3f *vel;
    Vec3f *lastVel;
    float limV;
    int id;
    int neighBoids;
    bool flock;
    bool predator;
    std::vector<int> neighbours;
    std::vector<Vec3f> trail;
}boidStruct;


typedef struct obstacle{
    Vec3f *pos;
    float aura;
    float radius;
    float height;
}obstacle;

typedef struct goalCube{
    Vec3f *pos;
    float size;
    Vec3f *vel;
}goalCube;


#endif /* defined(__FlockingFinal__Boid__) */
