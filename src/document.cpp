#include <iostream>
#include <fstream>
#include "../include/document.hpp"

using std::cout;
using std::endl;

Document::Document(string fn) {
    filename = fn;
    cout << "\033[H\x1b[J";
    load();
}
bool Document::get_save() {return is_saves;}
bool Document::get_work() {return is_work;}
void Document::stop_work() {is_work = false;}
bool Document::check_scroll() {
    bool changed = false;
    int visible_height = ws.ws_row - 4;
    int visible_width = ws.ws_col;

    if (curs.point_y >= curs.scroll_y + visible_height) {
        curs.scroll_y = curs.point_y - visible_height + 1;
        changed = true;
    }
    if (curs.point_y < curs.scroll_y) {
        curs.scroll_y = curs.point_y;
        changed = true;
    }

    if (curs.point_x >= curs.scroll_x + visible_width) {
        curs.scroll_x = curs.point_x - visible_width + 1;
        changed = true;
    }
    if (curs.point_x < curs.scroll_x) {
        curs.scroll_x = curs.point_x;
        changed = true;
    }
    return changed;
}
void Document::draw_ui() {
    string name = "voide 2.1";
    int pad_left = (ws.ws_col - (int)filename.length() - (int)name.length()) / 2;
    if (pad_left < 2) pad_left = 2;

    printf("\033[1;1H\x1b[47;30m %s%*s%s%*s\x1b[0m", 
           name.c_str(), pad_left - 2, "", filename.c_str(), 
           (int)(ws.ws_col - pad_left - filename.length() - name.length()), "");

    int row = ws.ws_row;
    printf("\033[%d;1H\x1b[47;30m^C\x1b[0m Exit \x1b[47;30m^S\x1b[0m Save \x1b[47;30m^O\x1b[0m Open\x1b[K", row);
    fflush(stdout);
    cursor_update();
}

void Document::draw_msg(string msg) {
    message = msg;
    int msg_row = ws.ws_row - 1;
        
    printf("\033[%d;1H", msg_row);
        
    if (message.empty()) {
        printf("\x1b[0m%*s", ws.ws_col, "");
    } else {
        size_t mess_len = message.length() + 4;
        int pad_left = (ws.ws_col - (int)mess_len) / 2;
        if (pad_left < 0) pad_left = 0;

        printf("\x1b[0m%*s\x1b[47;30m  %s  \x1b[0m\x1b[K", pad_left, "", message.c_str());
        fflush(stdout);
    }
        
    cursor_update();
}

void Document::cursor_update() {
    int screen_y = (curs.point_y - curs.scroll_y) + 2; 
    int screen_x = (curs.point_x - curs.scroll_x) + 1;
        
    if (screen_x < 1) screen_x = 1;
    if (screen_x > ws.ws_col) screen_x = ws.ws_col;

    cout << "\033[" << screen_y << ";" << screen_x << "H" << std::flush;
}
void Document::curs_line_update() {
    cout << "\x1b[K";
    cout << (lines[curs.point_y].c_str() + curs.point_x);
}
void Document::curs_doc_update() {
    check_scroll();
    string out = "\033[?25l";

    int visible_height = ws.ws_row - 4;
    // int visible_width = ws.ws_col;
    int screen_row = 0;

    for (int i = curs.scroll_y; i < (int)lines.size(); i++) {
        screen_row = (i - curs.scroll_y);
        if (screen_row >= visible_height) break;

        out += "\033[" + std::to_string(screen_row + 2) + ";1H\x1b[K";

        if ((int)lines[i].length() > curs.scroll_x) {
            string visible_text = lines[i].substr(curs.scroll_x, ws.ws_col);
            out += visible_text;
        }
            
        if (curs.scroll_x > 0 && i == curs.point_y) {
            out += "\033[" + std::to_string(screen_row + 2) + ";1H\x1b[1;33m$\x1b[0m";
        }
    }

    for (int j = screen_row + 1; j < visible_height; j++) {
        out += "\033[" + std::to_string(j + 2) + ";1H\x1b[K";
    }

    std::cout << out;
    draw_ui();
    draw_msg(message);
    std::cout << "\033[?25h" << std::flush;
}

void Document::add_char(char c) {
    if (curs.point_x == static_cast<int>(lines[curs.point_y].length())) {
        lines[curs.point_y].push_back(c);
    } else {
        lines[curs.point_y].insert(curs.point_x, 1, c);
    }
    curs.point_x++;
    is_saves = false;
        
    curs_doc_update();
}
void Document::back_char() {
    if(curs.point_x > 0) {
        lines[curs.point_y].erase(curs.point_x - 1, 1);
        curs.point_x--;
        cursor_update();
        curs_line_update();
        cursor_update();
        is_saves = false;
    } else if (curs.point_y > 0) {
        int target_y = curs.point_y - 1;
        int old_len = static_cast<int>(lines[target_y].length());

        lines[target_y] += lines[curs.point_y];
        lines.erase(lines.begin() + curs.point_y);

        curs.point_y--;
        curs.point_x = old_len;
            
        curs_doc_update();
        is_saves = false;
    }
}
void Document::del_char() {
    if(curs.point_x < static_cast<int>(lines[curs.point_y].length())) {
        lines[curs.point_y].erase(curs.point_x, 1);
        cursor_update();
        curs_line_update();
        cursor_update();
        is_saves = false;
    } else if(curs.point_y + 1 < static_cast<int>(lines.size())) {
        lines[curs.point_y] += lines[curs.point_y + 1];
        lines.erase(lines.begin() + curs.point_y + 1);
        curs_doc_update();
        is_saves = false;
    }
}

void Document::cursor_xp() {
    if(curs.point_x == static_cast<int>(lines[curs.point_y].length()) && curs.point_y + 1 < static_cast<int>(lines.size())) {
        curs.point_x = 0;
        curs.point_y++;
        if (check_scroll()) {
            curs_doc_update();
        }
        else {
            cursor_update();
        }
    } else if(curs.point_x + 1 <= static_cast<int>(lines[curs.point_y].length())) {
        curs.point_x++;
        if (check_scroll()) {
            curs_doc_update();
        }
        else {
            cursor_update();
        }
    }
}

void Document::cursor_xm() {
    if(curs.point_x == 0 && curs.point_y > 0) {
        curs.point_y--;
        curs.point_x = static_cast<int>(lines[curs.point_y].length());
        if (check_scroll()) {
            curs_doc_update();
        }
        else {
            cursor_update();
        }
    } else if(curs.point_x > 0) {
        curs.point_x--;
        if (check_scroll()) {
            curs_doc_update();
        }
        else {
            cursor_update();
        }
    }
}
void Document::cursor_yp() {
    if (curs.point_y + 1 < static_cast<int>(lines.size())) {
        curs.point_y++;
        if (curs.point_x > static_cast<int>(lines[curs.point_y].length())) 
            curs.point_x = static_cast<int>(lines[curs.point_y].length());
        
        if (check_scroll()) {
            curs_doc_update();
        } else {
            cursor_update();
        }
    } else if(curs.point_y+1 == static_cast<int>(lines.size())) {
        curs.point_x = static_cast<int>(lines[curs.point_y].length());
        cursor_update();
    }
}

void Document::cursor_ym() {
    if (curs.point_y > 0) {
        curs.point_y--;
        if (curs.point_x > static_cast<int>(lines[curs.point_y].length())) 
            curs.point_x = static_cast<int>(lines[curs.point_y].length());
        
        if (check_scroll()) {
            curs_doc_update();
        } else {
            cursor_update();
        }
    }
}

void Document::new_line() {
    string tail(lines[curs.point_y].c_str() + curs.point_x);
    lines[curs.point_y].erase(curs.point_x);
    lines.insert(lines.begin() + curs.point_y + 1, tail);
    curs.point_y++;
    curs.point_x = 0;
    curs_doc_update();
    is_saves = false;
}
void Document::tab() {
    for(int i = 0; i < 4; i++) {
        add_char(' ');
        cursor_update();
    }
}
void Document::save() {
    std::ofstream out(filename);
    if (!out.is_open()) {
        draw_msg("Error: Could not open file for saving!");
        return;
    }

    for (const auto& line : lines) {
        out << line << "\n";
    }
        
    out.close();
    draw_msg("Saved to " + filename);
    is_saves = true;
}
void Document::load() {
    std::ifstream in(filename);
    if (!in.is_open()) {
        draw_msg("New file: " + filename);
        curs_doc_update();
        return;
    }

    lines.clear();
    string temp;
    while (std::getline(in, temp)) {
        lines.push_back(temp);
    }

    if (lines.empty()) lines.push_back("");
    curs_doc_update();
}