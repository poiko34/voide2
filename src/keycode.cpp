#include "../include/keycode.hpp"

#ifdef _WIN32
    #include <conio.h> // Для _getch() и _kbhit()
#else
    #include <unistd.h>
#endif

int key_code(char& c) {
    // 1. Обработка для Windows
#ifdef _WIN32
    // На Windows Ctrl+C обычно ловится как 3, но стрелки идут через 0 или 224
    if (c == 3)   return K_EXIT;
    if (c == 19)  return K_SAVE;
    if (c == 8)   return K_BACKSPACE;
    if (c == 9)   return K_TAB;
    if (c == 13)  return K_ENTER;

    // Если c == 0 или 224 (0xE0), значит нажата спецклавиша
    if ((unsigned char)c == 0 || (unsigned char)c == 224) {
        // Читаем второй байт кода клавиши
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
    // 2. Обработка для Linux (ANSI-последовательности)
    if (c == 3)   return K_EXIT;      
    if (c == 19)  return K_SAVE;      
    if (c == 127 || c == 8) return K_BACKSPACE;
    if (c == 9)   return K_TAB;
    if (c == 13)  return K_ENTER;

    if (c == 27) { 
        char seq[3];
        // Важно: в Raw Mode read(STDIN_FILENO) будет ждать, пока не придут байты.
        // Чтобы не зависнуть на чистом ESC, в идеале тут проверяют доступность байтов через poll() или select().
        // Но для базы оставим чтение последовательности:
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