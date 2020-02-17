#include "task.h"

Task::Task()
{

    item = std::vector<int>();
    location.x = 0;
    location.y = 0;
    location.z = 0;
    urgency = 0;
    reachability = 0;
    quantity = std::vector<int>();
    ID = "";
    status = "available";
    weight = 0;
}

Task::Task(std::vector<int> _item, float x, float y, float z, int _urgency, int _reachability, std::vector<int> _quantity, std::string _ID)
{

    item = _item;
    location.x = x;
    location.y = y;
    location.z = z;
    urgency = _urgency;
    reachability = _reachability;
    quantity = _quantity;
    ID = _ID;
    status = "available";

    this->computeWeight();
}

void Task::computeWeight()
{
    weight = 0; //initialized in case size = 0
    for(std::size_t i = 0; i < item.size(); i ++)
    {
        switch(item[i])
        {
        case 1:
            weight += 1.5*quantity[i];
            break;
        case 2:
        case 3:
            weight += quantity[i];
            break;
        case 4:
            weight += 5*quantity[i];
            break;
        case 5:
            weight += 20*quantity[i];
            break;
        case 6:
            weight += quantity[i];
            break;
        default: //do nothing
            break;
        }
    }
}

std::string Task::getStatus(){ return status;}
int Task::getUrgency(){return urgency;}
int Task::getReachability(){return reachability;}
Point3D Task::getTasklLocation(){return location;};
float Task::getWeight(){ return weight;}
std::string Task::getID(){return ID;}
