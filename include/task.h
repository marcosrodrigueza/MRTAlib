#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include "custom_types.h"


class Task
{
private:

    std::vector<int> item;
    Point3D location;
    int urgency;
    int reachability;
    std::vector<int> quantity;

    std::string status;
    std::string ID;
    float weight;

    void computeWeight();

public:
    Task();
    Task(std::vector<int> _item, float x, float y, float z, int _urgency, int _reachability, std::vector<int> _quantity, std::string _ID);
    std::string getStatus();
    int getUrgency();
    int getReachability();
    Point3D getTasklLocation();
    float getWeight();
    std::string getID();
};

#endif // TASK_H
