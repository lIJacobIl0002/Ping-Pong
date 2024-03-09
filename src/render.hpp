#pragma once
// the main file where everything gets constructed together

#include "window.hpp"
#include "input.hpp"
#include "objects.hpp"
#include <cmath> // For std::pow and std::sqrt

void Window::render_background(unsigned color) {
    unsigned* pixel = this->memory;

    for(int i = 0; i < this->width; i++)
        for(int j = 0; j < this->height; j++)
            *pixel++ = color;
}

int clamp(int val, int min, int max) {
    if(val < min) return min;
    if(val > max) return max;

    return val;
}

void Window::Rectangle(Vector2 cords, Vector2 base, unsigned color) {
    unsigned* pixel;
    base.x += cords.x;
    base.y += cords.y;
    //clamp(cords);
    cords.x = clamp(cords.x, 0, this->width);
    cords.y = clamp(cords.y, 0, this->height);
    base.x = clamp(base.x, 0, this->width);
    base.y = clamp(base.y, 0, this->height);

    for(int y = cords.y; y < base.y; y++) {
        pixel = this->memory + cords.x + y*this->width;
        for(int x = cords.x; x < base.x; x++) {
            *pixel++ = color;
        }
    }
}

// "When I wrote this, only God and I understood what I was doing. Now, God only knows." - Karl Weierstrass
void Window::Circle(Vector2 center, int radius, unsigned color) {
    // Ensure radius is positive
    if(radius <= 0) return;

    int radiusSquared = radius * radius;

    int left = center.x - radius;
    int right = center.x + radius;
    int top = center.y - radius;
    int bottom = center.y + radius;

    left = clamp(left, 0, this->width);
    right = clamp(right, 0, this->width-radius);
    top = clamp(top, 0, this->height);
    bottom = clamp(bottom, 0, this->height-radius);

    for(int y = top; y <= bottom; y++) {
        for(int x = left; x <= right; x++) {
            int dx = x - center.x;
            int dy = y - center.y;
            if (dx*dx + dy*dy <= radiusSquared) {
                unsigned* pixel = this->memory + x + y * this->width;
                *pixel = color;
            }
        }
    }
}
