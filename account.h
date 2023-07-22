#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <openssl/md5.h>
#include <string>
#include "taskmanager.h"
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>
class Account {
public:
    // 构造函数
    Account();
    ~Account();
    void solve();
    // 登录函数
    bool login(const std::string& username, const std::string& password);

    bool changePassword( std::string& newPassword) ;
    
    void checkTasks();

private:
    // 加密函数
    std::string encrypt(const std::string& password);

    // 检查口令函数
    bool checkPassword(const std::string& password, const std::string& hash);
    
    // 存储账户信息的文件名
    static const std::string ACCOUNT_FILE;
    bool Flag;
    std::thread t;
    TaskManager taskManager;
    std::string storedUsername, storedHash,fileAddress;
};
std::string getPassword();

#endif // ACCOUNT_H
