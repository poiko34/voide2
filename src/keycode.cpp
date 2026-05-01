#include "../include/keycode.hpp"

#ifdef _WIN32
    #include <conio.h>
#else
    #include <unistd.h>
#endif

int key_code(char& c) {
#ifdef _WIN32
    if (c == 3)   return K_EXIT;
    if (c == 19)  return K_SAVE;
    if (c == 8)   return K_BACKSPACE;
    if (c == 9)   return K_TAB;
    if (c == 13)  return K_ENTER;

    if ((unsigned char)c == 0 || (unsigned char)c == 224) {
        int second = _getch(); 
        switch (second) {
            case 72: return K_UP;
            case 80: return K_DOWN;
            case 75: return K_LEFT;
            case 77: return K_RIGHT;
            case 83: return K_DELETE;
        }
    }
#else
    if (c == 3)   return K_EXIT;      
    if (c == 19)  return K_SAVE;      
    if (c == 127 || c == 8) return K_BACKSPACE;
    if (c == 9)   return K_TAB;
    if (c == 13)  return K_ENTER;

    if (c == 27) { 
        char seq[3];
        if (read(0, &seq[0], 1) != 1) return K_NONE;
        if (read(0, &seq[1], 1) != 1) return K_NONE;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return K_UP;
                case 'B': return K_DOWN;
                case 'C': return K_RIGHT;
                case 'D': return K_LEFT;
                case '3': 
                    if (read(0, &seq[2], 1) == 1 && seq[2] == '~') 
                        return K_DELETE;
            }
        }
    }
#endif

    return K_NONE;
}