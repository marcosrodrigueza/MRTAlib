#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <string>

class Allocation
{
private:

    std::string robot_id;
    std::string task_id;
    float registered_bid;


public:

    Allocation(std::string robot, std::string task, float bid);
    Allocation();
    std::string getTaskID();

};

#endif // ALLOCATION_H
