#include "account.h"

const std::string Account::
ACCOUNT_FILE = "accounts.txt";

Account::Account(): Flag(true){}
Account::~Account(){
    if (t.joinable()) t.join();
}

void Account::solve(){
    std::string username, password;
    std::cout << "Please enter your username: ";
    std::getline(std::cin, username);
    std::cout << "Please enter your password: ";
    password=getPassword();
    if(!login(username,password)){
        std::cout<<">>> Fail to login, returning to menu! >_< \n";
        return;
    }
    t = std::thread(&Account::checkTasks,this);
    system("clear");
    while(1)
    {
        std::string line(50, '-');
        std::cout<<line<<std::endl<<"[Logged in]\n"<<"1: Manage tasks\n2: Change password\n3: Quit\n\n"<<"Please choose one mode of work\n$ ";
        std::string getstr;
        std::getline(std::cin, getstr);
        
        if(getstr=="1") {
            taskManager.run();
        }
        else if(getstr=="2"){
            std::cout<<"Please input new password"<<std::endl;
            std::string newpassword;
            std::getline(std::cin, newpassword);
            while(newpassword.empty()){std::getline(std::cin, newpassword);}
            if(changePassword(newpassword)){
                system("clear");
                std::cout<<">>> Password successfully changed! ^_^\n";
            }
        }
        else if(getstr=="3") {
            Flag = false;
            t.join();
            std::cout<<"Successfully quit\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            system("clear");
            
            return;
        }
        else if(getstr.empty()){
            system("clear");
            continue;
        }
        else{
           system("clear");
           std::cout<<"Invalid input!\n"; 
        }
    }
    return;
}
    

bool Account::login(const std::string& username, const std::string& password) {
    // 加载账户信息
    std::ifstream accountsFile(ACCOUNT_FILE);
    if (!accountsFile.is_open()) {
        system("clear");
        std::cerr << "No account has been created yet!" << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(accountsFile, line)) {
        std::istringstream iss(line);
        
        if (iss >> storedUsername >> storedHash>>fileAddress) {
            if (storedUsername == username) {
                accountsFile.close();
                // 检查口令
                std::string hash = encrypt(password);
                if (!checkPassword(hash, storedHash)) return false;
                /* 在loadtask中读取,此处不再需要打开文件
                std::fstream file(fileAddress);
                if (file.is_open()) {
                    std::cerr << "Failed to open info file: " << fileAddress << "\n";
                    return flase;
                }
                */
                if(!taskManager.loadTasks(fileAddress)){
                    std::cerr << "Failed to open info file: " << fileAddress << "!\n";
                    return false;
                }
                return  true;
            }
        }
    }
    system("clear");
    std::cerr<<"Input of username doesn't exits!"<<std::endl;
    accountsFile.close();
    return false;
}

std::string Account::encrypt(const std::string& password) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)password.c_str(), password.size(), digest);
    char mdString[33];
    for (int i = 0; i < 16; ++i) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
    }
    return std::string(mdString);
}

bool Account::checkPassword(const std::string& password, const std::string& hash) {
    if (password == hash) return true;
    system("clear");
    std::cout<<"Input password is wrong!"<<std::endl;
    return false;
}

bool Account::changePassword(std::string& newPassword) 
{
    newPassword = encrypt(newPassword);
    if (storedHash== newPassword) {
        system("clear");
        std::cerr << "New password cannot be the same as the current password!" << std::endl;
        return false;
    }
    storedHash = newPassword;

    std::ifstream accountsFile(ACCOUNT_FILE);
    std::vector<std::string> lines;
    std::string line;
    if (!accountsFile.is_open()) {
        system(("touch "+ACCOUNT_FILE).c_str());
        accountsFile.open(ACCOUNT_FILE);
        if (!accountsFile.is_open()){
            std::cerr << "Failed to open account file !" << std::endl;
            return 0;
        }  
    }
    while (std::getline(accountsFile, line)) {
        lines.push_back(line);
    }
    for (size_t i = 0; i < lines.size(); i++) {
        std::istringstream iss(lines[i]);
        std::string s1, s2, s3;
        if (iss >> s1 >> s2 >> s3 && s1 == storedUsername) {
            lines[i] = s1 + " " + storedHash + " " + s3;
            break;
        }
    }
    accountsFile.close();    
    std::ofstream accountsFileWrite(ACCOUNT_FILE);
    for (const auto& line : lines) {
        accountsFileWrite << line << std::endl;
    }
     accountsFileWrite.close();    

    return true;
} 

void Account::checkTasks()
{
    while(Flag){
        while(!taskManager.m.try_lock()){
        }
        for (auto it = taskManager.getbeginTasks(); it != taskManager.getendTasks(); ++it) {
   		    if (it->getReminderTime() <= std::time(nullptr) && it->state) {
                std::string output = "notify-send --urgency=critical \"Reminder: ["+it->getName()+"] has expired.\"";
                system(output.c_str());
                std::cout<<"\n--------------------------------------------------\n";
       			std::cout << "Reminder: [" << it->getName() << "] has expired.";
                std::cout<<"\n--------------------------------------------------\n";
        		it->state = 0;
                taskManager.saveTasks();  		
            }
        }
        taskManager.m.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return;
}


std::string getPassword() {
    std::string hash;
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);  // 保存终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 关闭规范模式和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // 设置终端
    while (true)
	{
		ch = getchar();  // 读取单个字符
		if (ch=='\r'||ch=='\n'||ch==0)//getch()函数如果读到回车符号返回'/r'
		{
			break;
		}
            if (ch == 127) // 处理backspace字符
            {
                if (hash.size() > 0)
                {
                    hash.erase(hash.size() - 1);
                    putchar('\b'); // 光标回退一格
                    putchar(' '); // 输出一个空格覆盖之前的字符
                    putchar('\b'); // 再回退一格
                    fflush(stdout); 
                }
            }
    else 
        {
            hash += ch;
		    putchar('*');
            fflush(stdout); 
        }
	}
    std::cout<<std::endl;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 恢复终端设置
    return hash;
}
