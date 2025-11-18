#include <iostream>
#include <cmath>
#include "./Noise.h"

Vector2 randomGradient(int ix, int iy) {
    const unsigned int w = 8 * sizeof(unsigned int);
    const unsigned int s = w / 2;
    unsigned int a = ix, b = iy;
    a *= 3284157443u;
    b ^= a << s | a >> (w - s);
    b *= 1911520717u;
    a ^= b << s | b >> (w - s);
    a *= 2048419325u;
    float random = a * (3.14159265f / ~(~0u >> 1));  
    Vector2 v;
    v.x = std::sin(random);
    v.y = std::cos(random);
    return v;
}

float dotGridGradient(int ix, int iy, float x, float y) {
    Vector2 gradient = randomGradient(ix, iy);
    float dx = x - static_cast<float>(ix);
    float dy = y - static_cast<float>(iy);
    return (dx * gradient.x + dy * gradient.y);
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);  
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float perlinNoise(int _x , int _y , int mapWidth , int mapHeight) {
    float x = (float)_x/mapWidth;
    float y = (float)_y/mapHeight;
    int x0 = static_cast<int>(x); 
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float xf = x - static_cast<float>(x0);  
    float yf = y - static_cast<float>(y0);

    float u = fade(xf);
    float v = fade(yf);

    float n00 = dotGridGradient(x0, y0, x, y);
    float n01 = dotGridGradient(x0, y1, x, y);
    float n10 = dotGridGradient(x1, y0, x, y);
    float n11 = dotGridGradient(x1, y1, x, y);

    float ix0 = lerp(n00, n10, u);
    float ix1 = lerp(n01, n11, u);

    float value = lerp(ix0, ix1, v);

    return value;
}
