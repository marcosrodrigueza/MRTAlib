#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <vector>
#include "custom_types.h"
#include "task.h"

class Robot
{
 private:

    std::string type;
    float payload;
    float speed;
    Point3D location;
    float autonomy;
    float charge_time;

    float distance_travelled;
    float energy_level;

    float maximum_distance;
    Task current_tasks;
    std::vector<float>min_bid;
    std::string status;
    Task completed_tasks;
    int number_of_charges;
    float time_to_charge;
    Point3D summary_of_moments;
    std::vector<std::pair<std::string, float>> my_urgent_bids;
    std::vector<std::pair<std::string, float>> my_important_bids;
    std::vector<std::pair<std::string, float>> my_not_important_bids;

    void updateMaxDistance();

 public:
    std::string ID;
    Robot(std::string _type, float _payload, float _speed, float x, float y, float z, float _autonomy, float _charge_time, std::string _ID);
    float getMaxDistance();
    std::string getStatus();
    Point3D getLocation();
    float getPayload();
    float getSpeed();
    float getEnergy();
    std::string getID();
    void setStatus(std::string);
    void pushBid(std::string taskid, float distance, Urgency urgency);
    std::string getBestBid(Urgency urgency);
    float getBidValue(std::string id, Urgency urgency);
    void removeBid(std::string id, Urgency urgency);
    void charge(float ctime_secs); //time charging in seconds
    void discharge(float ctime_secs);//time charging in seconds
    void clearBids();
};

#endif // ROBOT_H
