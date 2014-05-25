//
//  Vec3f.h
//  FlockingFinal
//
//  Created by Joseph Milsom on 7/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//

#ifndef __FlockingFinal__Vec3f__
#define __FlockingFinal__Vec3f__

#include <iostream>
#include <math.h>

class Vec3f{
    
    public :
    Vec3f(float, float, float);
    //~Vec3f();
    Vec3f operator=(Vec3f);
    Vec3f operator+(Vec3f);
    Vec3f operator+=(Vec3f);
    Vec3f operator/(Vec3f);
    Vec3f operator/=(Vec3f);
    Vec3f operator/(float);
    Vec3f operator/=(float);
    Vec3f operator-(Vec3f);
    Vec3f operator-();
    Vec3f operator-=(Vec3f);
    Vec3f operator*(Vec3f);
    Vec3f operator*=(Vec3f);
    Vec3f operator*(float);
    Vec3f operator*=(float);
    float distance(Vec3f);
    float dotProduct(Vec3f);
    Vec3f* crossProduct(Vec3f);
    float magnitude();
    void init();
    float x;
    float y;
    float z;
    
    
};

#endif /* defined(__FlockingFinal__Vec3f__) */
