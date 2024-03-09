#include "window.hpp"

namespace Keys {

    int UP_ARROW = VK_UP;
    int DOWN_ARROW = VK_DOWN;
    int W_KEY = 'W';
    int S_KEY = 'S';

    int offset = 0x8000;
}

/*
    * Checks if the key is being pressed down or not
    @returns true if `key` is being pressed else false
*/
bool IsKeyPressed(int key) { // key handled here;
    return GetKeyState(key) & Keys::offset;
}
