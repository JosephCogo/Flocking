//
//  BoidController.cpp
//  FlockingFinal
//
//  Created by Joseph Milsom on 5/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//



#include "BoidController.h"
#include <math.h>
#include <GLUT/GLUT.h>
#include "Boid.h"
#include <iostream>
#include <vector>
#include "Vec3f.h"

using namespace std;


BoidController::BoidController(){
    d = BASIC;
    m = REDUP;
    centre = new Vec3f(0,0,0);
    sep = new Vec3f(0,0,0);
    objV = new Vec3f(0,0,0);
    pVel = new Vec3f(0,0,0);
    goal = new Vec3f(0,0,0);
    goalPos = new Vec3f(0,0,0);
    cube.pos = new Vec3f(0,0,0);
    cube.vel = new Vec3f(1,2,3);
    flocking = false;
    id = 0;
    neighbour = 5;
    separation = 2;
    bounds = 30;
    trailLength = 50;
    showHelp = true;
}

void BoidController::addBoids(int x){
    numberOfBoids += x;
    
    obstacle o;
    o.pos = new Vec3f{0,0,0};
    o.radius = 10;
    o.aura= 3;
    obstacleList.push_back(o);
    
    obstacle o2;
    o2.pos = new Vec3f{-15,-10,10};
    o2.radius = 5;
    o2.aura= 3;
    obstacleList.push_back(o2);
    
    obstacle o3;
    o3.pos = new Vec3f{10,-10,-10};
    o3.radius = 5;
    o3.aura= 3;
    obstacleList.push_back(o3);
    
    
    
    for(int i = 0; i < x; i++){
        boidStruct boid;
        
        float xp = (double)(rand()%300)/300.0f;
        float yp = (double)(rand()%300)/300.0f;
        float zp = (double)(rand()%300)/300.0f;
        //set position
        boid.pos = new Vec3f((xp*2-1)*20,(yp*2-1)*20,(zp*2-1)*20);
        for(int i = 0; i < 50; i++){
            boid.trail.push_back(*boid.pos);//initialise trail
        }
        //velocity vector
        float vx = (double)(rand()%100)/100.0f*2;
        float vy = (double)(rand()%100)/100.0f*2;
        float vz = (double)(rand()%100)/100.0f*2;
        boid.vel = new Vec3f(vx,vy,vz);
        boid.lastVel = new Vec3f(vx,vy,vz);//remove
        
        boid.id=id;
        boid.limV = ((double)(rand()%100)/100*2 +1);
        
        boid.flock = false;
        //create a predator
        if(i >= x-5 && x > 1){
            boid.predator =true;
        }
        else{
            boid.predator = false;
        }
        boids.push_back(boid);
        id++;
    }
}

void BoidController::centreMass(boidStruct *boid){
    centre->init();
    int neighBoids = 0;
    
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); itr++){
        if(boid->id != itr->id){
            //only calculate centre mass for boids that are close
            if(boid->pos->distance(*itr->pos) < neighbour && itr->predator == false){
                *centre += *(itr->pos);
                neighBoids++;
            }
        }
    }
    //calculates perceived centre of boids later by comparing boids
    if(neighBoids >0){
        boid->flock = true;
    }
    else{
        boid->flock = false;
    }
    if(centre->magnitude() != 0){
        *centre/= (neighBoids);
    }
}


void BoidController::keepDistance(boidStruct boid){
    sep->init();
    
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); ++itr){
        if(itr->id != boid.id){
            //calculate distance
            float distance = boid.pos->distance(*itr->pos);
            
            if(distance < separation){
                *sep -= *(itr->pos) - *(boid.pos);
            }
        }
    }
}

void BoidController::avoidObjects(boidStruct boid){
    objV->init();
    
    if(d == OBSTACLE){
    std::vector<obstacle>::iterator itr;
    for(itr = obstacleList.begin(); itr != obstacleList.end(); ++itr){
        float distance = boid.pos->distance(*itr->pos);
        if(distance < itr->radius+ itr->aura){
            *objV -= *(itr->pos) - *(boid.pos);
        }
    }
    }
    else{
            float distance = boid.pos->distance(*obstacleList[0].pos);
            if(distance < obstacleList[0].radius+ obstacleList[0].aura){
                *objV -= *(obstacleList[0].pos) - *(boid.pos);
            }
    }
}

//match velocity for flock
void BoidController::matchVelocity(boidStruct boid){
    pVel->init();
    int neighBoids=0;
    
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); ++itr){
        if(itr->id != boid.id){
            if(boid.pos->distance(*itr->pos) < neighbour && itr->predator == false){
                *pVel += *(itr->vel);
                neighBoids++;
            }
        }
    }
    //average velocity, then take an eighth to add to velocity
    if(pVel->magnitude() != 0){
        *pVel = (*pVel /(neighBoids));
        *pVel = (*pVel - *(boid.vel))/8;
    }
}

void BoidController::tendToPlace(boidStruct boid){
    if(d == ORBIT){
        goalPos = obstacleList[0].pos;
    }
    else{
        goalPos = cube.pos;
    }
    
    *goal = *goalPos - *boid.pos;
}

void BoidController::avoidPredators(boidStruct boid){
    goal->init();
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); ++itr){
        if(itr->id != boid.id){
            if(boid.pos->distance(*itr->pos) < 7.5 && boid.predator == false && itr->predator == true){
                *goal -= (*itr->pos - *boid.pos)/200;
            }
        }
    }
}

void BoidController::followTheLeader(boidStruct boid){
    if(boid.id > 0){
        *goalPos = *boids[boid.id-1].pos;
        *goal = (*goalPos -*boid.pos );
    }
}

void BoidController::limitVelocity(boidStruct *boid){
    float magnitude = boid->vel->magnitude();
    float limit = boid->limV;
    
    if(magnitude > boid->limV){
        *(boid->vel) = (*(boid->vel)/magnitude) * limit;
    }
}

void BoidController::boundBoids(boidStruct *boid){
    float area = bounds;
    float minX=-area;
    float minY = -area;
    float minZ = -area;
    float maxX =area;
    float maxY= area;
    float maxZ= area;
    //keep in bounds
    float bound = 0.05;
    
    if(boid->pos->x < minX){
        boid->vel->x += bound;
    }
    else if(boid->pos->x > maxX){
        boid->vel->x += -bound;
    }
    if(boid->pos->y < minY){
        boid->vel->y += bound;
    }
    else if(boid->pos->y > maxY){
        boid->vel->y += -bound;
    }
    if(boid->pos->z < minZ){
        boid->vel->z += bound;
    }
    else if(boid->pos->z > maxZ){
        boid->vel->z += -bound;
    }
}

void BoidController::boundCube(goalCube* cube){
    float area = bounds;
    float minX=-area;
    float minY = -area;
    float minZ = -area;
    float maxX =area;
    float maxY= area;
    float maxZ= area;
    //keep in bounds
    float bound = 0.05;
    
    if(cube->pos->x < minX){
        cube->vel->x += bound;
    }
    else if(cube->pos->x > maxX){
        cube->vel->x += -bound;
    }
    if(cube->pos->y < minY){
        cube->vel->y += bound;
    }
    else if(cube->pos->y > maxY){
        cube->vel->y += -bound;
    }
    if(cube->pos->z < minZ){
        cube->vel->z += bound;
    }
    else if(cube->pos->z > maxZ){
        cube->vel->z += -bound;
    }
}

void BoidController::enableFlocking(){
    flocking=true;
}

void BoidController::disableFlocking(){
    flocking=false;
}

void BoidController::move(){
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); ++itr){
        switch(d){
            case BASIC:
                centreMass(&(*itr));
                keepDistance(*itr);
                matchVelocity(*itr);
                if (flocking==true && itr->flock == true) {
                    *(itr->vel) += (*centre - *(itr->pos))/100 +(*sep/10)+(*pVel);
                }
                else  {
                    *(itr->vel) += *objV/20;
                }
                break;
            case PREDATOR:
                centreMass(&(*itr));
                keepDistance(*itr);
                matchVelocity(*itr);
                avoidPredators(*itr);//rename
                if (flocking==true && itr->flock == true && itr->predator == false) {
                    *(itr->vel) += (*centre - *(itr->pos))/100 +(*sep/10)+(*pVel)+(*goal) + *objV/20;
                }
                else  {
                    *(itr->vel) += *objV/20;;
                }
                break;
            case OBSTACLE:
                centreMass(&(*itr));
                keepDistance(*itr);
                matchVelocity(*itr);
                avoidObjects(*itr);
                if (flocking==true && itr->flock == true) {
                    *(itr->vel) += (*centre - *(itr->pos))/100 +(*sep/10)+(*pVel) + *objV/20;
                }
                else  {
                    *(itr->vel) += *objV/20;;
                }
                break;
            case COMBINED:
                centreMass(&(*itr));
                keepDistance(*itr);
                matchVelocity(*itr);
                avoidObjects(*itr);
                avoidPredators(*itr);
                if (flocking==true && itr->flock == true && itr->predator == false) {
                    *(itr->vel) += (*centre - *(itr->pos))/100 +(*sep/10)+(*pVel)+ (*goal)+ *objV/20;
                }
                else  {
                    *(itr->vel) += *objV/20;
                }
                break;
            case LEADER:
                centreMass(&(*itr));
                keepDistance(*itr);
                matchVelocity(*itr);
                followTheLeader(*itr);
                if(flocking == true && itr->id != 0){
                    *(itr->vel) += *goal;
                }
            case ORBIT:
                tendToPlace(*itr);
                keepDistance(*itr);
                matchVelocity(*itr);
                avoidObjects(*itr);
                if(flocking == true){
                    *(itr->vel) += *pVel + (*sep/10)+ (*goal/1000)+ *objV/20;
                }
            case GOAL:
                centreMass(&(*itr));
                tendToPlace(*itr);
                keepDistance(*itr);
                matchVelocity(*itr);
                if(flocking == true){
                    *(itr->vel) += *pVel + (*sep/10)+ (*goal/1000)+ *objV/20;
                }
            default:
                break;
        }
        
        boundBoids(&(*itr));
        limitVelocity(&(*itr));
        
        if(itr->predator == true && d == PREDATOR){
        *(itr->pos) += *(itr->vel)/2;
        }
        else{
        *(itr->pos) += *(itr->vel)/6;
        }
        itr->trail.push_back(*(itr->pos));
    }
    if(d== GOAL){
    boundCube(&cube);
    *cube.pos += *cube.vel/6;
    }
}


void BoidController::cycleColour(){
    if(m == REDUP){
        colour+=0.01;
        if(colour>1){
            m = GREENDOWN;
        }
    }
    else if(m == GREENDOWN){
        colour2-=0.01;
        if(colour2 < 0.3){
            m = BLUEUP;
        }
    }
    else if(m == BLUEUP){
        colour3+=0.01;
        if(colour3 > 1){
            m = REDDOWN;
        }
    }
    if(m == REDDOWN){
        colour-=0.01;
        if(colour < 0.3){
            m = GREENUP;
        }
    }
    else if(m == GREENUP){
        colour2+=0.01;
        if(colour2 > 1){
            m = BLUEDOWN;
        }
    }
    else if(m == BLUEDOWN){
        colour3-=0.01;
        if(colour3 < 0.3){
            m = REDUP;
        }
    }
}

void BoidController::drawTrail(boidStruct boid){
            glLineWidth(1);
            glBegin(GL_LINE_STRIP);
            float fade = 1;
            std::vector<Vec3f>::iterator p =boid.trail.end();
            --p;
            for(p; p >= boid.trail.begin(); --p){
                fade -= 1.0f/trailLength;
                if(boid.predator == true && (d == PREDATOR || d == COMBINED)){
                    glColor4f(1,1,1, fade);
                }
                else{
                glColor4f(colour, colour2, colour3, fade);
                }
                glVertex3f(p->x, p->y, p->z);
            }
            glEnd();
}


void BoidController::renderBoids(){
    move();
    cycleColour();
    Vec3f *z = new Vec3f(0,0,1);
    
    std::vector<boidStruct>::iterator itr;
    for(itr = boids.begin(); itr != boids.end(); ++itr){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glPushMatrix();
        
        if(itr->predator == true && (d == PREDATOR || d == COMBINED) ){
            glColor4f(1,1,1, 0.6);
        }else{
            glColor4f(colour, colour2, colour3, 0.6);
        }
        
        float theta = itr->vel->dotProduct(*z);
        float angle = acos (theta) * (180.0 / 3.14159265);
        Vec3f *cross = itr->vel->crossProduct(*z);
        
        glTranslatef(itr->pos->x, itr->pos->y, itr->pos->z);
        glRotatef(-angle, cross->x, cross->y, cross->z);
        
        //if predator draw bigger, if not draw normally
        if(itr->predator == true && (d == PREDATOR || d == COMBINED)){
            glutSolidCone(0.4, 2, 10, 10);
        }else{
            glutSolidCone(0.3, 1, 10, 10);
        }
        glPopMatrix();
        
        if(d == BASIC || d == LEADER || d == ORBIT){
            glDisable(GL_DEPTH_TEST);
            drawTrail(*itr);
        }
        
        itr->trail.erase(itr->trail.begin());
        delete[] cross;
    }
    glDisable(GL_BLEND);
    delete[] z;
    glColor3f(0.6,0.6,0.6);
    
    glEnable(GL_DEPTH_TEST);

    if(d == OBSTACLE || d== COMBINED){
    glPushMatrix();
    glTranslatef(obstacleList[0].pos->x, obstacleList[0].pos->y, obstacleList[0].pos->z);
    glutSolidSphere(obstacleList[0].radius, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(obstacleList[1].pos->x, obstacleList[1].pos->y, obstacleList[1].pos->z);
    glutSolidSphere(obstacleList[1].radius, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(obstacleList[2].pos->x, obstacleList[2].pos->y, obstacleList[2].pos->z);
    glutSolidSphere(obstacleList[2].radius, 50, 50);
    glPopMatrix();
    }else if (d == ORBIT){
        glPushMatrix();
        glTranslatef(obstacleList[0].pos->x, obstacleList[0].pos->y, obstacleList[0].pos->z);
        glutSolidSphere(obstacleList[0].radius, 50, 50);
        glPopMatrix();
    }
    if(d == GOAL){
        glPushMatrix();
        glTranslatef(cube.pos->x, cube.pos->y, cube.pos->z);
        glutSolidCube(2);
        glPopMatrix();
    }
    if(showHelp == true)
        writeText();
    
}

void BoidController::increaseSeparation(){
    separation +=0.1;
}

void BoidController::decreaseSeparation(){
    separation -=0.1;
}

void BoidController::increaseTrailLength(){
    trailLength+=1;
}

void BoidController::decreaseTrailLength(){
    trailLength-=1;
}

//overly verbose function!! SORRY
void BoidController::writeText(){
    int p = 724;
    glDisable(GL_LIGHTING);
    glColor3f(0,1,0);
    glWindowPos2d(0, p-=15);
    
    std::string s = "Mode: ";
    switch (d) {
        case BASIC:
            s += "Flocking Demonstration";
            break;
        case PREDATOR:
            s += "Predator Demonstration";
            break;
        case OBSTACLE:
            s += "Obstacle Avoidance Demonstration";
            break;
        case COMBINED:
            s += "Combined Demonstration";
            break;
        case LEADER:
            s += "Bonus! Trippy Swirly Particle Trails";
            break;
        case ORBIT:
            s += "Orbiting Demonstration";
            break;
        case GOAL:
            s += "Goal Seeking";
            break;
        default:
            break;
    }
    void * font = GLUT_BITMAP_9_BY_15;
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glWindowPos2d(0, p-=15);
    //number of boids
    s = "Number of Boids: ";
    s+= std::to_string(numberOfBoids);
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    //flocking
    glWindowPos2d(0, p-=15);
    s = "Flocking: ";
    if(flocking)
    s+= "Enabled";
    else
        s+= "Disabled";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    //separation
    glWindowPos2d(0, p-=15);
    s = "Separation: ";
    s+= std::to_string(separation);
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glWindowPos2d(0, p-=15*5);
    s = "Controls: ";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }

    glWindowPos2d(0, p-=15);
    s = "WASD to move, camera control with the mouse";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glWindowPos2d(0, p-=15);
    s = "1-7 change between the different modes ";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glWindowPos2d(0, p-=15);
    s = "m turns motion blur on and off";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glWindowPos2d(0, p-=15);
    s = "b adds a new boid";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    
    glWindowPos2d(0, p-=15);
    s = "c shows and hides the boundary cube";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glWindowPos2d(0, p-=15);
    s = "i increases separation while k decreases it";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glWindowPos2d(0, p-=15);
    s = "o increases the trail size while l decreases it";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glWindowPos2d(0, p-=30);
    s = "SPACE to hide help text";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glWindowPos2d(0, p-=15);
    s = "ESC to exit";
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glEnable(GL_LIGHTING);
}

int BoidController::getBounds(){
    return bounds;
}

void BoidController::changeMode(char key){
    switch (key) {
        case '1':
            d = BASIC;
            break;
        case '2':
            d = PREDATOR;
            break;
        case '3':
            d = OBSTACLE;
            break;
        case '4':
            d = COMBINED;
            break;
        case '5':
            d = GOAL;
            break;
        case '6':
            d = LEADER;
            break;
        case '7':
            d = ORBIT;
            break;
        default:
            break;
    }
    
}

void BoidController::showAndHideText(){
    if(showHelp == true){
        showHelp =false;
    }
    else{
        showHelp = true;
    }
    
}
