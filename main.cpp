#include "account.h"
#include <cstring>
#include <string>

const std::string ACCOUNT_FILE = "accounts.txt";

bool Sign_Up();


std::string Encrypt( std::string& password);

int main(int argc, char *argv[])
{
    std::string line(50, '-');
    
    system("clear");
    while(1){
        std::string input;
        std::cout<<line<<std::endl<<"1: Sign In"<<std::endl<<"2: Sign Up"<<std::endl<<"3: Quit"<<std::endl;
        std::cout<<"\nPlease choose one to work, input 1, 2 or 3"<<std::endl<<"$ ";
        std::getline(std::cin,input);
        if(input=="1")
        {
            Account account;
            account.solve();
        }
        else if(input=="2"){ 
            if(!Sign_Up()){
                std::cerr <<">>> Failed to sign up! >_<" << std::endl;
                }
            else{
                system("clear");
                std::cout<<">>> Successfully signed up! Please sign in. ^_^\n";
                }
            }
        else if (input == "3") {
            system("clear");
            exit(0);
            }
        else if(input.empty()){
            system("clear");
            continue;
            }
        else{
            system("clear");
            std::cout<<"Invalid input, the example input is: 1"<<std::endl;
        }
    }
    
    return 0;
}

bool Sign_Up(){
    std::ifstream accountsFile(ACCOUNT_FILE);
    
    if (!accountsFile.is_open()) {
        system(("touch "+ACCOUNT_FILE).c_str());
        accountsFile.open(ACCOUNT_FILE);
        if (!accountsFile.is_open()){
            std::cerr << "Failed to open account file !" << std::endl;
            return 0;
        }  
    }
    std::string line,storedUsername,inputUsername,hash;
    
    while(1)
    {
        bool Unique_username=1;
        std::cout<<"Input your Username, leave it blank to cancel signing up\n";
        std::getline(std::cin,inputUsername);
         for(int i = 0; i < inputUsername.length();i++){
            if (inputUsername[i]==' '){
                system("clear");
                std::cerr<<"Username can not contain space, please try again\n";
                accountsFile.close();
                return 0;
            }
        }
        if(inputUsername.empty())
        {   
            system("clear");
            std::cerr<<"Username should not be empty, please try again\n";
            accountsFile.close();
            return 0;
        } 
        while (std::getline(accountsFile, line)) {
            std::istringstream iss(line);
        
            if (iss >> storedUsername) {
                if (storedUsername ==inputUsername ) {
                    std::cerr<<"Username has been used by other users, please input another!\n"<<std::endl;
                    Unique_username=0;
                    break;
                }
            }
        }
        if(!Unique_username){continue;}
        std::cout<<"Input your Password, leave it blank to cancel signing up\n";
        
        hash=getPassword();
        
        if(hash.empty())
        {   
            accountsFile.close();
            return 0;
            //std::cerr<<"Password should not be empty, please input once again\n";
        } 
        accountsFile.close();
        hash=Encrypt(hash);
        std::ofstream WriteaccountsFile(ACCOUNT_FILE , std::ios::app);
        WriteaccountsFile <<inputUsername<< " " << hash;
        std::string fileaddress=hash+".txt";
        system(("touch "+fileaddress).c_str());
        // 将三个字符串变量写入文件
        WriteaccountsFile<< " " << fileaddress<< std::endl;
        WriteaccountsFile.close();
        return 1;
    }
    
}

std::string Encrypt( std::string& password) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)password.c_str(), password.size(), digest);
    char mdString[33];
    for (int i = 0; i < 16; ++i) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);
    }
    return std::string(mdString);
}

