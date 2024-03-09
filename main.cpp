#include "src/render.hpp"
#include <iostream>
//bool ARTIFICIAL_INTELLIGENCE = false;
bool checkCollision(Vector2 cords1, Vector2 base1, Vector2 cords2) {
    return (cords1.x <= cords2.x &&
    cords2.x <= (base1.x + cords1.x) &&
    cords1.y <= cords2.y &&
    cords2.y <= base1.y+cords1.y);
}

int main() {
    Window window = Window{"Window", 600, 1000};

    // had to do `struct` because `Rectangle` is defined
    struct Rectangle Player1({10, window.getHeight() / 2 - 100 / 2}, {10, 100});
    struct Rectangle Player2({window.getWidth() - 30, window.getHeight() / 2 - 100 / 2}, {10, 100});
    struct Circle Ball({window.getWidth() / 2 - 10 / 2, window.getHeight() / 2 - 10 / 2}, 5);
    
    int speed = 10;
    int ballSpeedX = 10, ballSpeedY = 0;
    srand(time(nullptr));

    window.Init();

    // Main loop
    while (window.IsOpen()) {
        window.render_background(0x0000FF);
        Player1.draw(&window, 0xFFFFFF);
        Player2.draw(&window, 0xFFFFFF);
        //window.Rectangle(pos1, {40, 100}, 0xFFFFFF);

        Player1.setY(clamp(Player1.getY(), 0, window.getHeight() - 100));
        Player2.setY(clamp(Player2.getY(), 0, window.getHeight() - 100));

        if(IsKeyPressed(Keys::W_KEY)) {
            Player1.setY(Player1.getY() + speed);
            //Player1.getY()+=speed; // fix. getters and setters should be proper
        }

        if(IsKeyPressed(Keys::S_KEY)) {
            Player1.setY(Player1.getY() - speed);
        }

        if(IsKeyPressed(Keys::DOWN_ARROW)) {
            Player2.setY(Player2.getY() - speed);
        }

        if(IsKeyPressed(Keys::UP_ARROW)) {
            Player2.setY(Player2.getY() + speed);
        }

        Ball.draw(&window, 0xFFFFFF);

        if(checkCollision(Player1.getPos(), Player1.getBase(), Ball.getPos()) ||
                    checkCollision(Player2.getPos(), Player1.getBase(), Ball.getPos())) {

            ballSpeedX *= -1;


            if (rand()%3 == 0 && ballSpeedY <= 10) {
                ballSpeedY += 5;
            } else if (ballSpeedY >= -10) {
                ballSpeedY -= 5;
            }
        }

        if(Ball.getY() <= 3 || Ball.getY() >= window.getHeight()-8){
            ballSpeedY *= -1;
        }

        // ball goes out of bound;
        if(Ball.getX() < 0 || Ball.getX() > window.getWidth()-5) {
            Player2 = {{window.getWidth() - 30, window.getHeight() / 2 - 100 / 2}, {10, 100}};
            Player1 = {{10, window.getHeight() / 2 - 100 / 2}, {10, 100}};

            Ball.setY(window.getHeight() / 2 - 10 / 2);
            Ball.setX(window.getWidth() / 2 - 10 / 2);

            ballSpeedX = 10; ballSpeedY = 0;

        }

        // update ball position
        Ball.setY(Ball.getY() + ballSpeedY);
        Ball.setX(Ball.getX() + ballSpeedX);

        Sleep(10);
    }


    return 0;
}
