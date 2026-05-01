#pragma once

#include <string>
#include <vector>
#include "terminal_manager.hpp"

using std::string;
using std::vector;

struct cursor {
    int point_x = 0, point_y = 0;
    int scroll_x = 0, scroll_y = 0;
};

class Document {
private:
    vector<string> lines{string("")};
    cursor curs;
    winsize ws = get_terminal_size();
    string filename;
    string message;
    bool is_saves = true;
    bool is_work = true;
public:
    Document(string fn);
    bool get_save();
    bool get_work();
    void stop_work();
    bool check_scroll();
    void draw_ui();

    void draw_msg(string msg);

    void cursor_update();
    void curs_line_update();
    void curs_doc_update();

    void add_char(char c);
    void back_char();
    void del_char();

    void cursor_xp();
    void cursor_xm();
    void cursor_yp();
    void cursor_ym();

    void new_line();
    void tab();
    void save();
    void load();
};