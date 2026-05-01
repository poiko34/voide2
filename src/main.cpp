#ifdef _WIN32
#include <io.h>
#include <conio.h>
#define STDIN_FILENO 0
#define read _read
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "../include/terminal_manager.hpp"
#include "../include/document.hpp"
#include "../include/keycode.hpp"
#include "../include/init.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./voide <file>" << std::endl;
        return 0;
    }

    std::ios_base::sync_with_stdio(false);
    TerminalManager term;
    std::cout << std::unitbuf;
    Document doc(argv[1]);

    char c;
    while (doc.get_work()) {

#ifdef _WIN32
        c = (char)_getch();
#else
        if (read(STDIN_FILENO, &c, 1) != 1) break;
#endif

        int code = key_code(c);

        if (code == K_EXIT) {
            if (!doc.get_save()) {
                doc.draw_msg("Are you sure? N - no, Y - yes");
                char confirm;
                while (1) {
#ifdef _WIN32
                    confirm = (char)_getch();
#else
                    read(STDIN_FILENO, &confirm, 1);
#endif
                    if (confirm == 'y' || confirm == 'Y') {
                        doc.stop_work();
                        break;
                    }
                    else if (confirm == 'n' || confirm == 'N') {
                        doc.draw_msg("");
                        doc.curs_doc_update();
                        break;
                    }
                    else cout << '\a';
                }
            }
            else {
                doc.stop_work();
            }
            continue;
        }

        switch (code) {
        case K_UP:        doc.cursor_ym(); break;
        case K_DOWN:      doc.cursor_yp(); break;
        case K_LEFT:      doc.cursor_xm(); break;
        case K_RIGHT:     doc.cursor_xp(); break;
        case K_ENTER:     doc.new_line();  break;
        case K_TAB:       doc.tab();       break;
        case K_BACKSPACE: doc.back_char(); break;
        case K_DELETE:    doc.del_char();  break;
        case K_SAVE:      doc.save();      break;

        case K_NONE:
            if ((unsigned char)c >= 32) {
                doc.add_char(c);
            }
            break;
        }
    }

    return 0;
}