#include "robot.h"

Robot::Robot(std::string _type, float _payload, float _speed, float x, float y, float z, float _autonomy, float _charge_time, std::string _ID)
    : current_tasks(), completed_tasks()
{
    type = _type;
    payload = _payload;
    speed = _speed;
    location.x = x;
    location.y = y;
    location.z = z;
    autonomy = _autonomy;
    charge_time = _charge_time;
    ID = _ID;

 //Non-passed values initialization

    distance_travelled = 0;
    energy_level = 100; //%energy
    maximum_distance = 0;

    //min bid defined with first push_back()
    status = "available";

    number_of_charges = 0;
    time_to_charge = 0;
    maximum_distance = speed*autonomy*60*(energy_level/100);
    summary_of_moments.x = 0;
    summary_of_moments.y = 0;
    summary_of_moments.z = 0;

}

void Robot::updateMaxDistance()
{
    maximum_distance = speed*autonomy*60*(energy_level/100);
}

float Robot::getMaxDistance(){ return maximum_distance;}
std::string Robot::getStatus(){ return status;}
Point3D Robot::getLocation(){return location;}
float Robot::getPayload(){return payload;}
float Robot::getSpeed(){return speed;}
float Robot::getEnergy(){return energy_level;}
std::string Robot::getID(){return ID;}


void Robot::setStatus(std::string stat)
{
    status = stat;

    if(stat == "comitted")
    {
        this->clearBids();
    }

}

void Robot::pushBid(std::string taskid, float distance, Urgency urgency)
{
    if(urgency == urgent)
        my_urgent_bids.push_back(std::make_pair(taskid,distance/speed));
    else if(urgency == important)
        my_important_bids.push_back(std::make_pair(taskid,distance/speed));
    else if(urgency == not_important)
        my_not_important_bids.push_back(std::make_pair(taskid,distance/speed));

}

std::string Robot::getBestBid(Urgency urgency)
{
    if(status == "comitted")
        return "0";

    if(urgency == urgent)
    {
        if(my_urgent_bids.size() > 0 )
        {
            std::vector<std::pair<std::string, float>>::iterator result = my_urgent_bids.begin();
            for(std::vector<std::pair<std::string, float>>::iterator i = my_urgent_bids.begin(); i != my_urgent_bids.end(); i++)
            {
                if(i->second < result->second)
                    result = i;
            }

            return result->first;
        }
        else
        {
            return "0";
        }
    }
    else if(urgency == important)
    {
        if(my_important_bids.size() > 0 )
        {
            std::vector<std::pair<std::string, float>>::iterator result = my_important_bids.begin();
            for(std::vector<std::pair<std::string, float>>::iterator i = my_important_bids.begin(); i != my_important_bids.end(); i++)
            {
                if(i->second < result->second)
                    result = i;
            }

            return result->first;
        }
        else
        {
            return "0";
        }
    }
    else if(urgency == not_important)
    {
        if(my_not_important_bids.size() > 0 )
        {
            std::vector<std::pair<std::string, float>>::iterator result = my_not_important_bids.begin();
            for(std::vector<std::pair<std::string, float>>::iterator i =my_not_important_bids.begin(); i != my_not_important_bids.end(); i++)
            {
                if(i->second < result->second)
                    result = i;
            }

            return result->first;
        }
        else
        {
            return "0";
        }
    }
    return "0";
 }

float Robot::getBidValue(std::string id, Urgency urgency)
{
    float result = 0;
    if(urgency == urgent)
    {
        if(my_urgent_bids.size() > 0 )
        {
            for(std::vector<std::pair<std::string, float>>::iterator i = my_urgent_bids.begin(); i != my_urgent_bids.end(); i++)
            {
                if(i->first == id)
                    result = i->second;
            }
        }
    }
    else if(urgency == important)
    {
        if(my_important_bids.size() > 0 )
        {
            for(std::vector<std::pair<std::string, float>>::iterator i = my_important_bids.begin(); i != my_important_bids.end(); i++)
            {
                if(i->first == id)
                    result = i->second;
            }
        }
    }
    else if(urgency == not_important)
    {
        if(my_not_important_bids.size() > 0 )
        {
            for(std::vector<std::pair<std::string, float>>::iterator i =my_not_important_bids.begin(); i != my_not_important_bids.end(); i++)
            {
                if(i->first == id)
                    result = i->second;
            }
        }
    }

    return result;
}

 void Robot::removeBid(std::string id, Urgency urgency)
 {
     if(urgency == urgent)
     {
         if(my_urgent_bids.size() > 0 )
         {
             for(std::vector<std::pair<std::string, float>>::iterator i = my_urgent_bids.begin(); i != my_urgent_bids.end(); i++)
             {
                 if(i->first == id)
                 {
                     my_urgent_bids.erase(i);
                     break;
                 }
             }
         }
     }
     else if(urgency == important)
     {
         if(my_important_bids.size() > 0 )
         {
             for(std::vector<std::pair<std::string, float>>::iterator i = my_important_bids.begin(); i != my_important_bids.end(); i++)
             {
                 if(i->first == id)
                 {
                    my_important_bids.erase(i);
                    break;
                 }
             }
         }
     }
     else if(urgency == not_important)
     {
         if(my_not_important_bids.size() > 0 )
         {
             for(std::vector<std::pair<std::string, float>>::iterator i =my_not_important_bids.begin(); i != my_not_important_bids.end(); i++)
             {
                 if(i->first == id)
                 {
                     my_not_important_bids.erase(i);
                     break;
                 }
             }
         }
     }

 }

 void Robot::charge(float ctime_secs)
 {
     if(ctime_secs > 0)
     {
         energy_level += (ctime_secs/(60*charge_time))*100;
     }
     if(energy_level > 100)
     {
         energy_level = 100;
     }

     this->updateMaxDistance();
 }

 void Robot::discharge(float ctime_secs)
 {
     if(ctime_secs > 0)
     {
         energy_level -= ((ctime_secs/60)/autonomy)*100;
     }
     if(energy_level < 0)
     {
         energy_level = 0;
         setStatus("lost");
     }

     this->updateMaxDistance();
 }

 void Robot::clearBids()
 {
     my_urgent_bids.clear();
     my_important_bids.clear();
     my_not_important_bids.clear();
 }
