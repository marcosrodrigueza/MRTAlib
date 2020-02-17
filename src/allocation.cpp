#include "allocation.h"

Allocation::Allocation()
{
    robot_id = "";
    task_id = "";
    registered_bid = -1;
}
Allocation::Allocation(std::string robot, std::string task, float bid)
{
    robot_id = robot;
    task_id = task;
    registered_bid = bid;
}
std::string Allocation::getTaskID(){return task_id;}
