#include "helper_functions.h"

void getDistance(Task& task,  std::vector<Robot*>& availRobots) //returns void, passed and modified by reference
{
    if(availRobots.size() > 0) //originally 0
    {
        Point3D task_location = task.getTasklLocation(); //  is defined as a member method for the single task

        std::vector<Robot*>::iterator it = availRobots.begin();

        while(it != availRobots.end())
        {
            Point3D r_loc = (*it)->getLocation();
            float euclidean_distance = euclideanDistance(r_loc.x, task_location.x, r_loc.y, task_location.y, r_loc.z, task_location.z);
            float robot_max_dist_capability = (*it)->getMaxDistance();
            if(2*euclidean_distance > robot_max_dist_capability)
            {
                //remove the element
              it =  availRobots.erase(it); //   "    "
               // it--;
            }
            else
            {
                it++;
            }

        } //end while
    }
   /* else
    {
        availRobots.clear();
    }*/
}

//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<float> getRobotsPayload(std::vector<Robot*>& availRobots)
{
    std::vector<float> payloads =  std::vector<float>();
    for(std::vector<Robot*>::iterator i = availRobots.begin(); i != availRobots.end(); i++)
    {
        payloads.push_back((*i)->getPayload());
    }

    return payloads;
}

//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void determineFinalAvailRobots(std::vector<Robot*>& robots, float weight)
{
    if(weight > 3 && weight <= 5)
    {
        std::vector<Robot*>::iterator it = robots.begin();

        while(it != robots.end())
        {
            if((*it)->ID == "1")
            {
               it = robots.erase(it); //// erase and update
            }

            else
            {
                it++;
            }
        } //end while
    }
    else if(weight > 5 && weight <= 20)
    {
        std::vector<Robot*>::iterator it = robots.begin();

        while(it != robots.end())
        {
            if((*it)->ID == "1" || (*it)->ID == "2" || (*it)->ID == "3")
            {
               it = robots.erase(it); //// erase and update
            }
            else
            {
                it++;
            }
        } //end while
    }
    else if(weight > 20 && weight <= 30)
    {
        std::vector<Robot*>::iterator it = robots.begin();

        while(it != robots.end())
        {
            if((*it)->ID == "1" || (*it)->ID == "2" || (*it)->ID == "3" || (*it)->ID == "4")
            {
                it = robots.erase(it); //// erase and update
            }
            else
            {
                it++;
            }
        } //end while
    }
    else if(weight > 30 && weight <= 40)
    {
        std::vector<Robot*>::iterator it = robots.begin();

        while(it != robots.end())
        {
            if((*it)->ID != "6")
            {
               it = robots.erase(it); //// erase and update
            }
            else
            {
                it++;
            }
        } //end while
    }
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<Task> loadTasks(std::string filepath)
{
    std::vector<Task> tasks;
    std::ifstream load_tasks;

    std::string item = "non-sense";
    std::string quantity = "non-sense";
    std::string reachability = "non-sense";
    std::string x = "non-sense";
    std::string y = "non-sense";
    std::string z = "non-sense";
    std::string urgency = "non-sense";
    std::string id = "non-sense";
    std::string num = "non-sense";
    //

    load_tasks.open(filepath, std::ios::in);

    if(!load_tasks)
    {
        return std::vector<Task>(0);
    }
    std::getline(load_tasks, num, '\n');
    for(int n = 0; n < std::stof(num); n++)
    {
        std::getline(load_tasks, x, ',');
        std::getline(load_tasks, y, ',');
        std::getline(load_tasks, z, ',');
        std::getline(load_tasks, urgency, ',');
        std::getline(load_tasks, reachability, ',');
        std::getline(load_tasks, item, ',');
        std::getline(load_tasks, quantity, ',');
        std::getline(load_tasks, id, '\n');

        std::istringstream iss(item), qss(quantity);

        std::vector<int> itm, quant;
        std::string temp = "non-sense";

        while(!iss.eof())
        {
           std::getline(iss, temp, '.');
           itm.push_back(std::stoi(temp));
        }
        // itm.erase(itm.end()-1);

        while(!qss.eof())
        {
           std::getline(qss, temp, '.');
           quant.push_back(std::stoi(temp));
        }
        //quant.erase(quant.end()-1);

        tasks.push_back(Task(itm,std::stof(x),std::stof(y),std::stof(z), std::stoi(urgency),std::stoi(reachability),quant,id));
    }
   // tasks.erase(tasks.end()-1);
    return tasks;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<Task>::iterator findTask(std::vector<Task>& taskvector, std::string task_id)
{
    std::vector<Task>::iterator it;
    for(it = taskvector.begin(); it != taskvector.end(); it++)
    {
        if(it->getID() == task_id)
            break;
    }

    return it;
}
