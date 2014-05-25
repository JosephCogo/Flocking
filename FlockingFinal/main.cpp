//
//  main.cpp
//  FlockingFinal
//
//  Created by Joseph Milsom on 5/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include "BoidController.h"
#include <ApplicationServices/ApplicationServices.h>


void writeText();
void initLight();
//camera position
float cameraX=0.0f;
float cameraY=0.0f;
float cameraZ=-50.0f;
//rotation
float rotateX=-1.0f;
float rotateY=-1.0f;
BoidController *boidsList;

bool moveForward = false;
bool moveBack = false;
bool moveLeft = false;
bool moveRight = false;
bool showCube = false;
bool flocking = false;

int lastX = 0;
int lastY = 0;

float width;
float height;

int n = 1;
int pos = 0;

void display(){
    float xRotTrad = (rotateX/180 * 3.141592654f);
    float yRotTrad = (rotateY/180 * 3.141592654f);
    if (moveForward) {
        cameraX -= float(sinf(yRotTrad));
        cameraY += float(sinf(xRotTrad));
        cameraZ += float(cosf(yRotTrad));
    }
    if (moveBack) {
        cameraX += float(sinf(yRotTrad));
        cameraY -= float(sinf(xRotTrad));
        cameraZ -= float(cosf(yRotTrad));
    }
    if (moveLeft) {
        cameraX += float(cosf(yRotTrad));
        cameraZ += float(sinf(yRotTrad));
    }
    if (moveRight) {
        cameraX -= float(cosf(yRotTrad));
        cameraZ -= float(sinf(yRotTrad));
    }

   // writeText();
    glLoadIdentity();
    initLight();

    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);

    //chase camera?
    glTranslatef(cameraX, 0, cameraZ);

    glClearColor(0, 0, 0, 1);
    
    glPushMatrix();
    boidsList->renderBoids();
    glPopMatrix();

    //draw boundary cube
    if(showCube){
        glColor3f(1, 1, 1);
        glutWireCube(boidsList->getBounds()*2);
    }

    //accumulation buffer, for fake motion blur
    if(pos == 0)
        glAccum(GL_LOAD, 1.0 / n);
    else
        glAccum(GL_ACCUM, 1.0 / n);
    pos++;
    if(pos >= n) {
        pos = 0;
        glAccum(GL_RETURN, 1.0);
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }

}

void initLight(){
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    
    float direction[] = {1, 0.2, -1, 0};//position of the spotlight
    float diffuse[] = {0.5,0.5,0.5,1};
    float specular[] = {1,1,1,1};

    glLightfv(GL_LIGHT0, GL_POSITION, direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 100 };
   
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    glEnable(GL_LIGHT0);
}

void setCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width/height, 1, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}



void keyboardFunction(unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
        case '1':
            boidsList->changeMode(key);
            break;
        case '2':
            boidsList->changeMode(key);
            break;
        case '3':
            boidsList->changeMode(key);
            break;
        case '4':
            boidsList->changeMode(key);
            break;
        case '5':
            boidsList->changeMode(key);
            break;
        case '6':
            boidsList->changeMode(key);
            break;
        case '7':
            boidsList->changeMode(key);
            break;
        case 'w':
            moveForward = true;
            break;
        case 's':
            moveBack=true;
            break;
        case 'a':
            moveLeft = true;
            break;
        case 'd':
            moveRight=true;
            break;
        case 'b':
            boidsList->addBoids(1);
            break;
        case 'i':
            boidsList->increaseSeparation();
            break;
        case 'k':
            boidsList->decreaseSeparation();
            break;
        case 'o':
            boidsList->increaseTrailLength();
            break;
        case 'l':
            boidsList->decreaseTrailLength();
            break;
        case 'z':
            glEnable(GL_DEPTH_TEST);
            break;
        case 'x':
            glDisable(GL_DEPTH_TEST);
            break;

        default:
            break;
    }
}

void keyReleaseFunction(unsigned char key, int x, int y){
    
    switch (key) {
        case 'w':
            moveForward = false;
            break;
        case 's':
            moveBack=false;
            break;
        case 'a':
            moveLeft = false;
            break;
        case 'd':
            moveRight=false;
            break;
        case 'm':
            if(n == 1)
                n = 3;
            else
                n = 1;
            break;
        case 'c':
            if(showCube == false)
                showCube = true;
            else
                showCube = false;
            break;
        case 'f':
            if(flocking == false){
                flocking = true;
                boidsList->enableFlocking();
            }
            else{
                flocking = false;
                boidsList->disableFlocking();
            }
            break;
        case ' ':
            boidsList->showAndHideText();
            break;
    }
}

void mouseMovement(int x, int y){
    int diffx=x-lastX;
    int diffy=y-lastY;
    
    lastX=x;
    lastY=y;

    if(diffx>100){
        diffx=0;
    }
    if(diffy>100){
        diffy=0;
    }
    rotateX += (float) diffy*0.25;
    rotateY += (float) diffx*0.25;
    if(x > 300){
        glutWarpPointer(200, y);
        lastX=200;
    }
    else if(x < 150){
        glutWarpPointer(200, y);
        lastX=200;
    }
    if(y > 300){
        glutWarpPointer(x, 200);
        lastY=200;
    }
    else if(y < 150){
        glutWarpPointer(x, 200);
        lastY=200;
    }
}

void timerFunc(int value){
    glutPostRedisplay();
    //60 fps
    glutTimerFunc(1000.0f/60.0f, timerFunc, 0);
}

void init(){
    glShadeModel(GL_SMOOTH);
    glEnable (GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    boidsList = new BoidController();
    boidsList->addBoids(500);
    //get rid of that pesky delay on a mac
    CGSetLocalEventsSuppressionInterval(0.0);
    setCamera();
    initLight();
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    width = glutGet(GLUT_SCREEN_WIDTH);
    height = glutGet(GLUT_SCREEN_HEIGHT);
    
    glutInitWindowSize(width, height);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Flocking Final");
    glutKeyboardFunc(keyboardFunction);
    glutKeyboardUpFunc(keyReleaseFunction);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(mouseMovement);
    glutTimerFunc(1000.0f/60.0f, timerFunc, 0);
    init();
    glutDisplayFunc(display);
    glutFullScreen();
    glutMainLoop();
}

