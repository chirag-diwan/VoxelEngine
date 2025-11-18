#ifndef NOISE_H
#define NOISE_H

#include <iostream>
#include <cmath>

struct Vector2 {
    float x, y;
};

Vector2 randomGradient(int ix, int iy) ;

float dotGridGradient(int ix, int iy, float x, float y) ;

float fade(float t) ;

float lerp(float a, float b, float t) ;

float perlinNoise(int _x , int _y , int mapWidth , int mapHeight) ;

#endif
