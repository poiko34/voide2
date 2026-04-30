#pragma once

enum Key {
    K_NONE = 0, K_EXIT = 1, K_SAVE = 2, K_BACKSPACE = 3,
    K_DELETE = 4, K_UP = 5, K_DOWN = 6, K_RIGHT = 7, K_LEFT = 8,
    K_TAB = 9, K_ENTER = 10
};

int key_code(char& c);