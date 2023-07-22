#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>
#include <mutex>
class Task {
public:
    // 构造函数
    Task();

    // 设置和获取任务属性
    void setName(const std::string& name);
    std::string getName() const;

    void setStartTime(const std::time_t& time);
    std::time_t getStartTime() const;

    void setPriority(int priority);
    int getPriority() const;

    void setCategory(const std::string& category);
    std::string getCategory() const;

    void setReminderTime(const std::time_t& time);
    std::time_t getReminderTime() const;

    void setId(int id);
    int getId() const;

    short state;

    void editTask(std::string &newtaskname, std::time_t &newstarttime, int newpriority,std::string &newcategory,std::time_t &newremindertime);

private:
    // 任务属性
    std::string name_;
    std::time_t startTime_;
    int priority_;
    std::string category_;
    std::time_t reminderTime_;
    int id_;
    //
};

#endif // TASK_H
