#include <stdio.h>
#include "../include/init.hpp"

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
    static DWORD orig_input_mode;
    static DWORD orig_output_mode;
    static UINT orig_cp;
#else
    static struct termios orig_termios;
#endif

void enableRawMode() {
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    orig_cp = GetConsoleOutputCP();
    GetConsoleMode(hIn, &orig_input_mode);
    GetConsoleMode(hOut, &orig_output_mode);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD rawIn = orig_input_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
    SetConsoleMode(hIn, rawIn);

    DWORD rawOut = orig_output_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    SetConsoleMode(hOut, rawOut);
#else
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;

    raw.c_iflag &= ~(IXON | ICRNL); 
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
}

void disableRawMode() {
#ifdef _WIN32
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), orig_input_mode);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), orig_output_mode);
    SetConsoleOutputCP(orig_cp);
    SetConsoleCP(orig_cp);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
#endif
}

void flushInput() {
#ifdef _WIN32
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}
