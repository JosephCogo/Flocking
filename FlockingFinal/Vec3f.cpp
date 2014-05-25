//
//  Vec3f.cpp
//  FlockingFinal
//
//  Created by Joseph Milsom on 7/10/13.
//  Copyright (c) 2013 Joseph Milsom. All rights reserved.
//

#include "Vec3f.h"


Vec3f::Vec3f(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3f Vec3f::operator=(Vec3f other){
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

Vec3f Vec3f::operator+(Vec3f other){
    Vec3f add = *this;
    add.x = this->x + other.x;
    add.y = this->y + other.y;
    add.z = this->z + other.z;
    return add;
}

Vec3f Vec3f::operator+=(Vec3f other){
    this->x = this->x + other.x;
    this->y = this->y + other.y;
    this->z = this->z + other.z;
    return *this;
}

Vec3f Vec3f::operator/(Vec3f other){
    Vec3f div = *this;
    div.x = this->x / other.x;
    div.y = this->y / other.y;
    div.z = this->z / other.z;
    return div;
}

Vec3f Vec3f::operator/=(Vec3f other){
    this->x = this->x / other.x;
    this->y = this->y / other.y;
    this->z = this->z / other.z;
    return *this;
}

Vec3f Vec3f::operator/(float scalar){
    Vec3f div = *this;
    div.x = this->x / scalar;
    div.y = this->y / scalar;
    div.z = this->z / scalar;
    return div;
}

Vec3f Vec3f::operator/=(float scalar){
    this->x = this->x / scalar;
    this->y = this->y / scalar;
    this->z = this->z / scalar;
    return *this;
}

Vec3f Vec3f::operator-(Vec3f other){
    Vec3f min = *this;
    min.x = this->x - other.x;
    min.y = this->y - other.y;
    min.z = this->z - other.z;
    return min;
}

Vec3f Vec3f::operator-=(Vec3f other){
    this->x = this->x - other.x;
    this->y = this->y - other.y;
    this->z = this->z - other.z;
    return *this;
}

Vec3f Vec3f::operator-(){
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
    return *this;
}

Vec3f Vec3f::operator*(Vec3f other){
    Vec3f mult = *this;
    mult.x = this->x * other.x;
    mult.y = this->y * other.y;
    mult.z = this->z * other.z;
    return mult;
}

Vec3f Vec3f::operator*=(Vec3f other){
    this->x = this->x * other.x;
    this->y = this->y * other.y;
    this->z = this->z * other.z;
    return *this;
}

Vec3f Vec3f::operator*(float multi){
    Vec3f mult = *this;
    mult.x = this->x * multi;
    mult.y = this->y * multi;
    mult.z = this->z * multi;
    return mult;
}

Vec3f Vec3f::operator*=(float mult){
    this->x = this->x * mult;
    this->y = this->y * mult;
    this->z = this->z * mult;
    return *this;
}

float Vec3f::distance(Vec3f other){
    float distance = sqrtf(pow(this->x - other.x, 2) + pow(this->y - other.y, 2) + pow(this->z - other.z, 2));
    return distance;
}

void Vec3f::init(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

float Vec3f::magnitude(){
    return sqrt(pow(this->x, 2) + pow(this->y, 2)+pow(this->z, 2));
}

float Vec3f::dotProduct(Vec3f other){
    return this->x/this->magnitude() * other.x/other.magnitude() + this->y/this->magnitude() * other.y/other.magnitude() + this->z/this->magnitude() * other.z/other.magnitude();
}

Vec3f* Vec3f::crossProduct(Vec3f other){
    float crossX = this->y/this->magnitude() * other.z/other.magnitude() - this->z/this->magnitude() * other.y/other.magnitude();
    float crossY = this->z/this->magnitude() * other.x/other.magnitude() - this->x/this->magnitude() * other.z/other.magnitude();
    float crossZ = this->x/this->magnitude() * other.y/other.magnitude() - this->y/this->magnitude()*other.x/other.magnitude();
    Vec3f *crossProd = new Vec3f(crossX, crossY, crossZ);
    return crossProd;
}
