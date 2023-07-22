#include "taskmanager.h"
#include "addtask.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

TaskManager::TaskManager()= default;
/*
 time_t now = time(0);
 cout << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << endl;
 */
bool AddInteract(string &taskname, time_t &starttime, int &priority,string &category,time_t &remindertime){
    string stday,sttime,rmday,rmtime;
    tm tm_time = {};
    starttime=time(0);
    remindertime=time(0)+86400;
    taskname="NewTask";
    if (localtime(&starttime)->tm_hour<10){taskname+='0';}
    taskname+=to_string(localtime(&starttime)->tm_hour);
    if (localtime(&starttime)->tm_min<10){taskname+='0';}
    taskname+=to_string(localtime(&starttime)->tm_min);
    if (localtime(&starttime)->tm_sec<10){taskname+='0';}
    taskname+=to_string(localtime(&starttime)->tm_sec);
    priority=1;
    category="Normal";
    ostringstream oss1,oss2;
    oss1<<put_time(localtime(&starttime), "%Y-%m-%d %H:%M:%S");
    oss2<<put_time(localtime(&remindertime), "%Y-%m-%d %H:%M:%S");
    string line=taskname+' '+oss1.str()+' '+oss2.str()+' '+ to_string(priority)+' '+category;
    cout<<"You're adding a new task to Task Master, please enter according to the prompt! Also make sure the window is wide enough to avoid display errors.\n";
    if(dynamic_edit(line)){cout<<"should never happen\n";}
    istringstream iss(line);
    if(iss>>taskname){}
    else{system("clear");cout<<"[Can't resolve taskname]\n";return 1;}
    if(iss>>stday){}
    else{system("clear");cout<<"[Can't resolve stday]\n";return 1;}
    if(iss>>sttime){}    
    else{system("clear");cout<<"[Can't resolve sttime]\n";return 1;}
    if(iss>>rmday){}
    else{system("clear");cout<<"[Can't resolve rmday]\n";return 1;}
    if(iss>>rmtime){}
    else{system("clear");cout<<"[Can't resolve rmtime]\n";return 1;}
    if(iss>>priority){}
    else{system("clear");cout<<"[Can't resolve priority]\n";return 1;}
    if(iss>>category){}
    else{system("clear");cout<<"[Can't resolve category]\n";return 1;}
    if(strptime((stday+" "+sttime).c_str(), "%Y-%m-%d %H:%M:%S", &tm_time)==nullptr){
        system("clear");
        cout<<"[Illegal starttime]\n";return 1;
    }
    starttime=mktime(&tm_time);
    if(strptime((rmday+" "+rmtime).c_str(), "%Y-%m-%d %H:%M:%S", &tm_time)==nullptr){
        system("clear");
        cout<<"[Illegal remindertime]\n";return 1;
    }
    remindertime=mktime(&tm_time);
    return 0;
}

void DisplayInteract(TaskManager &manager){
    string input,parameter;
    int index=0;
    cout<<"Please find or display tasks based on command usage:\n\n";
    cout<<"(leave it blank)\tDisplay all tasks in ascending order of ID.\n"
        <<"name <string>\t\tFind tasks with names containing the given string.\n"
        <<"prio \t\t\tDisplay all tasks in descending order of priority.\n"
        <<"prio <int>\t\tFind tasks set to the given priority.\n"
        <<"startdate \t\tDisplay tasks in ascending order of starttime\n"
        <<"startdate <date>\tFind tasks starting on this date, format of <date> is \"%Y-%m-%d\".\n"
        <<"duedate \t\tDisplay tasks in ascending order of duetime\n"
        <<"duedate <date>\t\tFind tasks due by this date, format of <date>is \"%Y-%m-%d\".\n"
        <<"category <string>\tDisplay tasks of this category.\n\n$ ";
    getline(cin, input,'\n');
    system("clear");
    if(input.empty()){manager.showTasks();}
    else if(input.substr(0,4)=="name"){
        index=4;
        while(input[index]==' '|| input[index]=='\t'){index++;}
        parameter=input.substr(index,input.length()-1);
        manager.showTasks(parameter);
    }
    else if(input.substr(0,4)=="prio"){
        index=4;
        while(input[index]==' '|| input[index]=='\t'){index++;}
        parameter=input.substr(index,input.length()-1);
        if(parameter=="") manager.showTaskspriority();
        else {
                if(all_of(parameter.begin(), parameter.end(), [](char c) { return isdigit(c); })){
                    manager.showTasks(stoi(parameter));
                }
                else{
                system("clear");
                cout<<">>> Illegal input! Priority must be an integer. >_< \n";
                }
            }
        }
    else if(input.substr(0,9)=="startdate"){
        index=9;
        while(input[index]==' '|| input[index]=='\t'){index++;}
        if(input[index]=='\0'){
            manager.showTasksbyST();
        }
        else{
            parameter=input.substr(index,input.length()-1);
            manager.showTasks(parameter,1);
        }
        
    }
    else if(input.substr(0,7)=="duedate"){
        index=7;
        while(input[index]==' '|| input[index]=='\t'){index++;}
        if(input[index]=='\0'){
            manager.showTasksbyRT();
        }
        else{
            parameter=input.substr(index,input.length()-1);
            manager.showTasks(parameter,2);
        }      
    }
    else if(input.substr(0,8)=="category"){
        index=8;
        while(input[index]==' '|| input[index]=='\t'){index++;}
        parameter=input.substr(index,input.length()-1);
        manager.showTasks(parameter,3);
    }
    else{
        system("clear");
        cout<<">>> Unrecognized command, returning to menu. >_< \n";
        }
    cout<<endl;
}

bool EditInteract(TaskManager &manager,int id){
    auto it = find_if(manager.getbeginTasks(), manager.getendTasks(), [id](const Task &task) {return task.getId() == id;});

    if (it == manager.getendTasks()) {
        return 1;
    }
    time_t st=it->getStartTime(),rt=it->getReminderTime();

    string newtaskname,newcategory,stday,sttime,rmday,rmtime;
    time_t newstarttime,newremindertime;
    tm tm_time = {};
    int newpriority;
    ostringstream oss;
    string line;
    oss<<it->getName()<<' '<<put_time(localtime(&st), "%Y-%m-%d %H:%M:%S")<<' '<<put_time(localtime(&rt), "%Y-%m-%d %H:%M:%S")<<' '<<to_string(it->getPriority())<<' '<<it->getCategory();
    line=oss.str();
    system("clear");
    cout<<"You're editting task: "<<it->getName()<<" , please enter according to the prompt! Also make sure the window is wide enough to avoid display errors.\n";
    if(dynamic_edit(line)){cout<<"should never happen\n";}
    istringstream iss(line);
    if(iss>>newtaskname){}
    else{system("clear");cout<<"[Can't resolve taskname]\n";return 1;}
    if(iss>>stday){}
    else{system("clear");cout<<"[Can't resolve stday]\n";return 1;}
    if(iss>>sttime){}    
    else{system("clear");cout<<"[Can't resolve sttime]\n";return 1;}
    if(iss>>rmday){}
    else{system("clear");cout<<"[Can't resolve rmday]\n";return 1;}
    if(iss>>rmtime){}
    else{system("clear");cout<<"[Can't resolve rmtime]\n";return 1;}
    if(iss>>newpriority){}
    else{system("clear");cout<<"[Can't resolve priority]\n";return 1;}
    if(iss>>newcategory){}
    else{system("clear");cout<<"[Can't resolve category]\n";return 1;}
    if(strptime((stday+" "+sttime).c_str(), "%Y-%m-%d %H:%M:%S", &tm_time)==nullptr){
        system("clear");
        cout<<"[Illegal starttime]\n";return 1;
    }
    newstarttime=mktime(&tm_time);
    if(strptime((rmday+" "+rmtime).c_str(), "%Y-%m-%d %H:%M:%S", &tm_time)==nullptr){
        system("clear");
        cout<<"[Illegal remindertime]\n";return 1;
    }
    newremindertime=mktime(&tm_time);
    it->editTask(newtaskname,newstarttime,newpriority,newcategory,newremindertime);
    return 0;

}

void TaskManager::run(){
    string opt,taskname,category;
    bool idflag=true,flag=true;
    time_t starttime,remindertime;
    int priority,id;
    Task *task;
    system("clear");
    string inputid;
    while(flag){
        string line(50, '-');
        cout<<line<<endl;
        cout<<"You're managing your tasks! Supported operations:\n"<<"1: add task\n"<<"2: delete task\n"<<"3: edit task\n"<<"4: display\n"<<"5: back\n\n"<<"Please select one:\n$ ";
        getline(cin,opt);
        if (opt.length()==1){
            switch (opt[0]){
                case '1':
                    system("clear");
                    if(AddInteract(taskname,starttime,priority,category,remindertime)){
                        cout<<">>> Failed to add this task! >_<\n";
                        break;
                    }
                    if(addTask(taskname,starttime,priority,category,remindertime)){
                        if(!saveTasks()){
                            cout<<"Failed to save tasks! Please check! \n";
                        }
                        system("clear");
                        cout<<">>> New task successfully added! ^_^ \n";
                    }
                    else{
                        cout<<">>> Failed to add this task, please retry! >_<\n";
                    }
                    break;
                case '2':
                    system("clear");
                    cout<<"Please enter the ID of any of the following tasks to delete it. Press enter to cancel.\n\n";
                    showTasks();
                    cout<<"\nID of task needed to be deleted: ";
                    idflag=true;
                    getline(cin,inputid);
                    if(inputid.length()==0){
                        system("clear");
                        cout<<"Delete operation cancelled!\n";
                        break;
                    }
                    for (char c : inputid) {
                        if (!std::isdigit(c)) {
                            idflag=false;
                        }
                    }
                    if(idflag && inputid.length()>0){
                        id = stoi(inputid);
                        task=findTaskById(id);
                        if (task){
                            if(!deleteTask(id)){
                                cout<<"Should never happen!\n";
                            }
                            else{
                                if(!saveTasks()){
                                    cout<<"Failed to save tasks! Please check! \n";
                                    break;
                                }
                                system("clear");
                                cout<<"Delete success!\n";
                                break;
                            }
                        }
                        else{
                            system("clear");
                            cout<<"No such task!\n";
                            break;
                        }
                    }
                    else {
                        system("clear");
                        cout<<">>> Bad ID! Returning to menu. >_<\n";
                        }
                    break;
                case '3':
                    system("clear");
                    cout<<"Please enter the ID of any of the following tasks to edit it. Press enter to cancel.\n\n";
                    showTasks();
                    cout<<"\nID of task you want to edit: ";
                    idflag=true;
                    getline(cin,inputid);
                    if(inputid.length()==0){
                        system("clear");
                        cout<<"Edit operation cancelled!\n";
                        break;
                    }
                    for (char c : inputid) {
                        if (!std::isdigit(c)) {
                            idflag=false;
                        }
                    }
                    if(idflag && inputid.length()>0){
                        id = stoi(inputid);
                        task=findTaskById(id);
                        if (task){
                            if(EditInteract(*this,id)){
                                cout<<"Should never happen!\n";
                            }
                            else{
                                if(!saveTasks()){
                                    cout<<"Failed to save tasks! Please check! \n";
                                    break;
                                }
                                system("clear");
                                cout<<"Edit success!\n";
                                break;
                            }

                        }
                        else{
                            system("clear");
                            cout<<"No such task!\n";
                            break;
                        }
                    }
                    else {
                        system("clear");
                        cout<<">>> Bad ID! Returning to menu. >_<\n";
                        }
                    break;
                case '4':
                    system("clear");
                    DisplayInteract(*this);
                    break;
                case '5':
                    system("clear");
                    return;
                default:
                    system("clear");
                    cout<<">>> Unrecognizable operation! Please re-enter! >_<\n$ ";
            }
        }
        else{
            system("clear");
            cout<<"Unrecognizable operation! Please re-enter!\n$ ";
        }
    }
    return;
}

//成功创建任务返回真，否则返回假
bool TaskManager::addTask(const string &name, const time_t &startTime,const int priority,
                          const string &category, const time_t &reminderTime) {
    // 创建新任务并调用类内函数设置该任务信息
    Task newtask;
    newtask.setName(name);
    newtask.setStartTime(startTime);
    newtask.setPriority(priority);
    newtask.setCategory(category);
    newtask.setReminderTime(reminderTime);
    // 检查该任务是否与已有任务冲突
    if (checkTaskConflict(newtask)) {
        return false;
    }
    newtask.setId(generateId());

    // 添加任务到本account的taskmanager的任务内存中
    
    while(!m.try_lock()){ 
    }
    tasks_.push_back(newtask);
    m.unlock();

   
    return true;
}

bool TaskManager::saveTasks() {
    // 保存任务到文件
    ofstream taskFile(TASK_FILE);
    if (!taskFile) {
        return false;
    }
    //遍历当前任务内存，逐行保存到任务数据文件中
    for (const auto &task : tasks_) {
        taskFile << task.getId() << " " << task.getName() << " " << task.getStartTime()
                 << " " << task.getPriority() << " " << task.getCategory() << " "
                 << task.getReminderTime() <<" "<<task.state<< endl;
    }
    return true;
}


bool TaskManager::deleteTask(int id) {
    // 查找任务
    auto it = find_if(tasks_.begin(), tasks_.end(), [id](const Task &task) {return task.getId() == id;});
    //遍历所有的任务也没有找到，删除失败
    if (it == tasks_.end()) {
        return false;
    }
    // 删除任务

    while(!m.try_lock()){
    }
    tasks_.erase(it);
    m.unlock();
   
    return true;
}

void displaySingleTask(const Task &task){
    time_t starttime=task.getStartTime(),remindertime=task.getReminderTime();
    cout<<task.getId()<<"\t"<<task.getName()<<"\t"<< put_time(localtime(&starttime), "%Y-%m-%d %H:%M:%S")<<"\t"<< put_time(localtime(&remindertime), "%Y-%m-%d %H:%M:%S")<<"\t"<<task.getPriority()<<"\t"<<task.getCategory()<<"\t";
    if(task.state==0){cout<<"(expired)";}
    cout<<endl;
}

void TaskManager::showTasks(const string &attribute, const int type) {
    // 显示指定日期的任务,type为1时attribute是开始日期,type为2时attribute的是截止日期,type为3时attribute是category
    if(type==1){
        sort(tasks_.begin(),tasks_.end(), compareByStart);
        cout << "Tasks starting on " << attribute << ":" << endl;

        struct tm st {};
        strptime(attribute.c_str(), "%Y-%m-%d", &st);

        for (const auto &task : tasks_) {
            time_t starttime=task.getStartTime();
            if (st.tm_year == localtime(&starttime)->tm_year && st.tm_mon == localtime(&starttime)->tm_mon && st.tm_mday == localtime(&starttime)->tm_mday){
                displaySingleTask(task);
            }
        }
    }
    else if(type==2){
        sort(tasks_.begin(),tasks_.end(), compareByDue);
        cout << "Tasks due by " << attribute << ":" << endl;

        struct tm st {};
        strptime(attribute.c_str(), "%Y-%m-%d", &st);

        for (const auto &task : tasks_) {
            time_t remindertime=task.getReminderTime();
            if (st.tm_year == localtime(&remindertime)->tm_year && st.tm_mon == localtime(&remindertime)->tm_mon && st.tm_mday == localtime(&remindertime)->tm_mday){
                displaySingleTask(task);
            }
        }
    }
    else if(type==3){
        cout << "Tasks of type " << attribute << ":" << endl;
        for (const auto &task : tasks_) {
            if(task.getCategory()==attribute){
                displaySingleTask(task);
            }
        }
    }
}

void TaskManager::showTasks(int priority){
    sort(tasks_.begin(),tasks_.end(), compareById);
    cout<<"Task of priority "<<priority<<":"<<endl;
    for (const auto &task : tasks_) {
        if(task.getPriority()==priority){
            displaySingleTask(task);
        }
    }
}

void TaskManager::showTasks(const string &partofname) {
    sort(tasks_.begin(),tasks_.end(), compareByName);
    cout<<"Task containing \""<<partofname<<"\", in alphabetical order:"<<endl;
    for (const auto &task : tasks_) {
        if(task.getName().find(partofname)!=string::npos){
            displaySingleTask(task);
        }
    }
}


void TaskManager::showTasks() {
    // 以ID顺序显示所有任务
    sort(tasks_.begin(),tasks_.end(), compareById);
    cout<<"All tasks: "<<endl;
    for (const auto &task : tasks_) {
        displaySingleTask(task);
    }
}


void TaskManager::showTaskspriority(){
    std::sort(tasks_.begin(), tasks_.end(), [](const Task& a, const Task& b) {
        return a.getPriority() < b.getPriority(); // 按照priority从小到大排序
    });
    cout<<"All tasks in descending order of priority: "<<endl;
    for (const auto &task : tasks_) {
        displaySingleTask(task);
    }

}


void TaskManager::showTasksbyST(){
    std::sort(tasks_.begin(), tasks_.end(), [](const Task& a, const Task& b) {
        return a.getStartTime() < b.getStartTime(); // 按照priority从小到大排序
    });
    cout<<"All tasks in ascending order of starttime: "<<endl;
    for (const auto &task : tasks_) {
        displaySingleTask(task);
    }


}

void TaskManager::showTasksbyRT(){
    std::sort(tasks_.begin(), tasks_.end(), [](const Task& a, const Task& b) {
        return a.getReminderTime() < b.getReminderTime(); // 按照priority从小到大排序
    });
    cout<<"All tasks in ascending order of remindertime: "<<endl;
    for (const auto &task : tasks_) {
        displaySingleTask(task);
    }

}


Task* TaskManager::findTaskById(int id) {
    // 根据id查找任务
    auto it = find_if(tasks_.begin(), tasks_.end(), [id](const Task &task) {
        return task.getId() == id;
    });
    if (it == tasks_.end()) {
        return nullptr;
    }
    return &(*it);
}

int TaskManager::generateId() {
    // 生成唯一id
    int id = 1;
    for (const auto &task : tasks_) {
        if (task.getId() >= id) {
            id = task.getId() + 1;
        }
    }
    return id;
}

bool TaskManager::checkTaskConflict(const Task &task) {
    // 检查任务是否有冲突
    for (const auto &t : tasks_) {
        if (t.getName() == task.getName() && t.getStartTime() == task.getStartTime()) {
            return true;
        }
    }
    return false;
}

Task*  TaskManager::getbeginTasks(){
    return tasks_.data();
}
Task*  TaskManager::getendTasks(){
    return tasks_.data()+tasks_.size();
}

 bool TaskManager::loadTasks(const string& filepath){

    TASK_FILE=filepath;
    fstream taskFile(filepath);
    if (!taskFile.is_open()) {
        return false;
    }
    string line;
    while (getline(taskFile, line))
    {
        istringstream iss(line);
        int id, priority;
        time_t startTime, reminderTime;
        short state;
        string name, category;
        if (iss >> id >> name >> startTime >> priority >> category >> reminderTime>>state) {
            Task task;
            task.setId(id);
            task.setName(name);
            task.setStartTime(startTime);
            task.setPriority(priority);
            task.setCategory(category);
            task.setReminderTime(reminderTime);
            task.state=state;
            tasks_.push_back(task);
        }
    }
    return true;
 }

bool compareById(const Task &a, const Task &b) {
    return a.getId() < b.getId();
}

bool compareByDue(const Task &a, const Task &b) {
    return a.getReminderTime() < b.getReminderTime();
}

bool compareByStart(const Task &a, const Task &b) {
    return a.getStartTime() < b.getStartTime();
}

bool compareByName(const Task &a, const Task &b) {
    return a.getName() < b.getName();
}
