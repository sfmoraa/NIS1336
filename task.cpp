#include "task.h"

Task::Task() :state(1){}

void Task::setName(const std::string& name) {
    name_ = name;
}
std::string Task::getName() const {
    return name_;
}

void Task::setStartTime(const std::time_t& time) {
    startTime_ = time;
}

std::time_t Task::getStartTime() const {
    return startTime_;
}

void Task::setPriority(int priority) {
    priority_ = priority;
}

int Task::getPriority() const {
    return priority_;
}

void Task::setCategory(const std::string& category) {
    category_ = category;
}

std::string Task::getCategory() const {
    return category_;
}

void Task::setReminderTime(const std::time_t& time) {
    reminderTime_ = time;
}

std::time_t Task::getReminderTime() const {
    return reminderTime_;
}

void Task::setId(int id) {
    id_ = id;
}

int Task::getId() const {
    return id_;
}

void Task::editTask(std::string &newtaskname, std::time_t &newstarttime, int newpriority,std::string &newcategory,std::time_t &newremindertime){
    name_=newtaskname;
    startTime_=newstarttime;
    priority_=newpriority;
    category_=newcategory;
    reminderTime_=newremindertime;
}
