#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include "task.h"

class TaskManager {
public:
    // 构造函数
    TaskManager();

    // 任务录入
    bool addTask(const std::string &name, const std::time_t &startTime, int priority,
                 const std::string &category, const std::time_t &reminderTime);

    // 任务保存
    bool saveTasks();

    // 任务加载
    bool loadTasks(const std::string &filename);

    // 任务删除
    bool deleteTask(int id);

    // 任务显示
    void showTasks(const std::string &attribute, int type);
    void showTasks(int priority);
    void showTasks(const std::string &partofname);
    void showTasks();
    void showTasksbyST();
    void showTasksbyRT();
    void showTaskspriority();

    // 根据id查找任务
    Task* findTaskById(int id);
    Task* getbeginTasks();
    Task* getendTasks();

    void run();
    std::mutex m;


private:
    // 任务列表
    std::vector<Task> tasks_;

    // 任务数据文件名
    std::string TASK_FILE;

    // 生成唯一id
    int generateId();

    // 检查任务是否有冲突
    bool checkTaskConflict(const Task &task);
};
void displaySingleTask(const Task &task);

bool compareById(const Task &a, const Task &b) ;

bool compareByDue(const Task &a, const Task &b) ;

bool compareByStart(const Task &a, const Task &b);

bool compareByName(const Task &a, const Task &b);

#endif // TASKMANAGER_H