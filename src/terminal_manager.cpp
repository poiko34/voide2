#include <iostream>
#include "../include/terminal_manager.hpp"
#include "../include/init.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

// Реализация методов класса
TerminalManager::TerminalManager() {
    enableRawMode();
}

TerminalManager::~TerminalManager() {
    // Очистка экрана: \033[H - курсор в начало, \033[2J - очистить всё
    std::cout << "\033[H\033[2J" << std::flush;
    disableRawMode();
}

// Кросплатформенное получение размера окна
struct winsize get_terminal_size() {
    struct winsize ws;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hOut, &csbi)) {
        ws.ws_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        ws.ws_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        ws.ws_col = 80;
        ws.ws_row = 24;
    }
#else
    // Используем STDOUT_FILENO (обычно 1) для запроса к терминалу
    if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        ws.ws_col = 80;
        ws.ws_row = 24;
    }
#endif
    return ws;
}