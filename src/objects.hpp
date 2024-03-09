#include "window.hpp"

class Parent {
    public:
    Vector2 cords{};

    int getX() const {
        return cords.x;
    }

    int getY() const {
        return cords.y;
    }

    void setY(int y) {
        cords.y = y;
    }

    void setX(int x) {
        cords.x = x;
    }

    Vector2 getPos() const {
        return cords;
    }
};

struct Rectangle : public Parent {
    Vector2 base{};

    public:

    Rectangle(Vector2 cords, Vector2 base) {
        this->cords = cords;
        this->base = base;
    }

    Vector2 getBase() const {
        return this->base;
    }

    void draw(Window* window, unsigned int color) const {
        window->Rectangle(cords, base, color);
    }


};

struct Circle : public Parent {
    int radius;

    public:
    Circle(Vector2 cords, int radius) : radius(radius) {
        this->cords = cords;        
    }

    void draw(Window* window, unsigned int color) const {
        window->Circle(this->cords, this->radius, color);
    }
};

