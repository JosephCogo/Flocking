//
//  BoidController.h
//  FlockingFinal
//
//  Created by Joseph Milsom on 5/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//

#ifndef __FlockingFinal__BoidController__
#define __FlockingFinal__BoidController__

#include <iostream>
#include "Boid.h"
#include <vector>

//enum for the current colour cycle
enum MODE{REDUP, REDDOWN, BLUEUP, BLUEDOWN, GREENUP, GREENDOWN};
enum DISPLAYMODE{BASIC, PREDATOR, OBSTACLE, COMBINED, LEADER, ORBIT, GOAL};

class BoidController{
    
private:
    //define private methods
    void move();
    void centreMass(boidStruct*);
    void keepDistance(boidStruct);
    void avoidObjects(boidStruct);
    void matchVelocity(boidStruct);
    void avoidPredators(boidStruct);
    void tendToPlace(boidStruct);
    void followTheLeader(boidStruct);
    void limitVelocity(boidStruct*);
    void boundBoids(boidStruct*);
    void boundCube(goalCube*);
    void cycleColour();
    void drawTrail(boidStruct);
    void writeText();
    
    //define private variables
    std::vector<boidStruct> boids;
    std::vector<obstacle> obstacleList;
    float colour = 0; //variables for cycling colours
    float colour2 = 1;
    float colour3 = 0;
    MODE m; //for controlling the cycle colour mode
    DISPLAYMODE d; //control the boid movement mode
    int numberOfBoids;
    Vec3f *centre;
    Vec3f *sep;
    Vec3f *objV;
    Vec3f *pVel;
    Vec3f *goal;
    Vec3f *goalPos;
    goalCube cube;
    bool flocking;
    int id;
    int neighbour;
    double separation;
    int bounds;
    float trailLength;
    bool showHelp;
    
public:
    BoidController(void);
    void enableFlocking();
    void disableFlocking();
    void increaseSeparation();
    void decreaseSeparation();
    void increaseTrailLength();
    void decreaseTrailLength();
    void addBoids(int);
    void renderBoids();
    int getBounds();
    void changeMode(char key);
    void showAndHideText();

    
};




#endif /* defined(__FlockingFinal__BoidController__) */
