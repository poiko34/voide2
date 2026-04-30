#pragma once

#ifdef _WIN32
    struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
    };
#else
    #include <sys/ioctl.h>
#endif

class TerminalManager {
public:
    TerminalManager();
    ~TerminalManager();
};

struct winsize get_terminal_size();