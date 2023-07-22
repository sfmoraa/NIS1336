#include  <stdio.h>
#include  <stdlib.h>
#include  <termios.h>
#include  <iostream>
#include "addtask.h"

static  struct termios oldt;
using namespace std;

// 恢复终端属性
void reset_terminal_mode() {
    tcsetattr( 0 , TCSANOW,  & oldt);
}

// 设置终端属性
void set_terminal_mode() {
    struct  termios newt;

    // save terminal settings
    tcgetattr( 0 ,  & oldt);
    // init new settings
    newt  =  oldt;
    // change settings
    newt.c_lflag  &=   ~ (ICANON  |  ECHO);
    // apply settings
    tcsetattr( 0 , TCSANOW,  & newt);
    // make sure settings will be restored when program ends
    atexit(reset_terminal_mode);
}

// 将光标移动到指定位置
void move_cursor(int pos) {
    cout << "\033[" << pos + 1 << "G";
}

void help(int mod,int pos){// 0为初态，1为insert，2为insert态下按了一次Esc，3为初态下按了一次Esc
    if(mod==0){
        cout<<"\33[1A\r\33[K";
        cout<<"(Press 'i' to Insert, press 'Esc' twice to exit.)\33[1B";
        move_cursor(pos);
    }
    else if(mod==1){
        cout<<"\33[1A\r\33[K";
        cout<<"(Edit below, and press 'Esc' twice to exit Insert mode.)\33[1B";
        move_cursor(pos);
    }
    else if(mod==2){
        cout<<"\33[1A\r\33[K";
        cout<<"(You have pressed 'Esc' once, press again to exit Insert mode.)\33[1B";
        move_cursor(pos);
    }
    else if(mod==3){
        cout<<"\33[1A\r\33[K";
        cout<<"(You have pressed 'Esc' once, press again to finish.)\33[1B";
        move_cursor(pos);
    }
}
// 输出字符串
void print_string(const string &str, int pos) {
    cout << str << "\033[" << str.size() - pos << "D";
}

int dynamic_edit(string &str) {
    // 初始化字符串和光标位置
    int pos = 0;

    // 设置终端属性
    set_terminal_mode();
    cout<<"Task format is: <Name> <StartTime> <ReminderTime> <Priority> <Category>\n\n";
    cout<<"(Press 'i' to Insert, press 'Esc' twice to exit.)\n";
    // 输出字符串
    print_string(str, pos);

    int ch;
    while (1) {
        ch  =  getchar();
        if (ch == 27) { // 双击Esc 键，则退出程序
            help(3,pos);
            ch=getchar();
            if(ch==27){
                cout<<'\n';
                break;
            }
            else{
                help(0,pos);
                if(ch==91){
                    ch=getchar();
                    if (ch == 68 && pos > 0) { // 光标左移
                        pos--;
                        move_cursor(pos);
                    }
                    else if (ch == 67 && pos < str.size()) { // 光标右移
                        pos++;
                        move_cursor(pos);
                    }
                }
            }
        }
        else if (ch == 'i') { // 插入模式
            int c;
            help(1,pos);
            while (1) { // 直到插入结束或 ESC 被按下
                c=getchar();
                if (c==27){
                    help(2,pos);
                    ch=getchar();
                    if(ch==27){
                        help(0,pos);
                        break;
                    }
                    else{
                        help(1,pos);
                        if(ch==91){
                            ch=getchar();
                            if (ch==51){
                                ch=getchar();
                                if(ch==126){
                                    if(pos<str.size()){
                                        str.erase(pos,1);
                                        printf( "\r\33[K"); // 清空当前行输出
                                        print_string(str, pos); // 输出更改后的字符串
                                    }
                                }
                            }
                            else{
                                if (ch == 68 && pos > 0) { // 光标左移
                                    pos--;
                                    move_cursor(pos);
                                }
                                else if (ch == 67 && pos < str.size()) { // 光标右移
                                    pos++;
                                    move_cursor(pos);
                                }
                            }
                        }
                    }
                }
                else if(c==127){
                    if(pos>0){
                        str.erase(pos-1,1);
                        pos--;
                        printf( "\r\033[K"); // 清空当前行输出
                        print_string(str, pos); // 输出更改后的字符串
                    }
                }
                else if(c==10){
                    cout<<"\r\033[K";
                    print_string(str, pos);
                }
                else{
                    str.insert(pos, 1, static_cast<char>(c));
                    pos++;
                    printf( "\r\033[K"); // 清空当前行输出
                    print_string(str, pos); // 输出更改后的字符串
                }
            }
        }
    }

    // 恢复终端属性
    reset_terminal_mode();
    return 0;
}