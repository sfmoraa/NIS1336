//
// Created by Lenovo on 2023/7/3.
//

#ifndef ADDTASK_H
#define ADDTASK_H

#include <string>

void reset_terminal_mode();
void set_terminal_mode();
void move_cursor(int pos);
void help(int mod,int pos);
void print_string(const std::string &str, int pos);
int dynamic_edit(std::string &str);


#endif //ADDTASK_H
