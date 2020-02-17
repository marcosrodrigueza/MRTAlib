  #include "taskmanager.h"

TaskManager::TaskManager()
{
    //empty constructor
}

TaskManager::TaskManager(std::vector<Robot> swarm)
{
    robots = swarm;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::addRobot(const Robot& r)
{
    robots.push_back(r);
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

bool TaskManager::removeRobot(std::string r)
{
    bool done = false;
    for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end(); i++)
    {
        if(i->getID() == r)
        {
            robots.erase(i);
            done = true;
            break;
        }
    }
    return done;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

bool TaskManager::removeRobot(Robot r)
{
    std::string id = r.getID();
    bool done = false;
    for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end(); i++)
    {
        if(i->getID() == id)
        {
            robots.erase(i);
            done = true;
            break;
        }
    }
    return done;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________


bool TaskManager::loadRobots(std::string filepath)
{
    std::ifstream load_robots;

    std::string type = "non-sense";
    std::string payload = "non-sense";
    std::string speed = "non-sense";
    std::string x = "non-sense";
    std::string y = "non-sense";
    std::string z = "non-sense";
    std::string autonomy = "non-sense";
    std::string charge_time = "non-sense";
    std::string id = "non-sense";
    //

    load_robots.open(filepath, std::ios::in);

    if(!load_robots)
    {
        return false;
    }

    while(!load_robots.eof())
    {
        std::getline(load_robots, type, ',');
        std::getline(load_robots, payload, ',');
        std::getline(load_robots, speed, ',');
        std::getline(load_robots, x, ',');
        std::getline(load_robots, y, ',');
        std::getline(load_robots, z, ',');
        std::getline(load_robots, autonomy, ',');
        std::getline(load_robots, charge_time, ',');
        std::getline(load_robots, id, '\n');

        robots.push_back(Robot(type,std::stof(payload),std::stof(speed),std::stof(x), std::stof(y),std::stof(z),
                               std::stof(autonomy),std::stof(charge_time),id));
    }
    //last element(duplicated) removed
    robots.erase(robots.end()-1);
    load_robots.close();
    return true;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

 void TaskManager::respawnRobots()
 {
    for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
    {
        it->setStatus("available");
        it->clearBids();
        it->charge(100000);
    }
 }
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<Task> TaskManager::generateSampleTasks(int n)
{
    if(robots.size() < 1)
    {
        return std::vector<Task>(0);
    }

    //else

    srand(time(NULL));
    std::vector<float> max_distances;
    std::vector<Robot>::iterator searcher;

    float min_of_max = robots[0].getMaxDistance(); //initialization of min value
    int i = 0;

    for(searcher = robots.begin(); searcher != robots.end(); searcher ++) //Copy the values to a vector
    {
        max_distances.push_back(searcher->getMaxDistance());
    }

    min_of_max = ((*std::min_element(max_distances.begin(), max_distances.end()))/2)*0.7071;

    std::vector<Point3D> points = std::vector<Point3D>();
    float location_upper_limit = min_of_max;
    float location_lower_limit = 0 - min_of_max;

    for(i = 0; i < n; i++)
    {
        float x = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;
        float y = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;
        float z = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;

          if (i>0)
          {
             int j = 0;
              while(j < points.size()) //Looks for coincidences
              {
                  if (points[j].x == x && points[j].y == y && points[j].z == z)
                  {
                      x = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;
                      y = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;
                      z = (2*location_upper_limit)*((float)rand()/RAND_MAX) + location_lower_limit;
                      j = 0;
                  }
                  else
                  {
                      j++;
                  }
              }
          }
          Point3D temp_point;
          temp_point.x = x;
          temp_point.y = y;
          temp_point.z = z;
          points.push_back(temp_point);
    }

    const int item_lower_limit = 1;
    const int item_upper_limit = 6;

    const int urgency_lower_limit = 1;
    const int urgency_upper_limit = 3;

    const int reachability_lower_limit = 1;
    const int reachability_upper_limit = 3;

    const int quantity_lower_limit = 1;
    const int quantity_upper_limit = 10;

    std::vector<Task> generated_tasks;

    i = 1;

    while(i <= n)
    {
       int urgency = std::round((urgency_upper_limit-urgency_lower_limit)*((float)rand()/RAND_MAX) + urgency_lower_limit);
       int reachability =  std::round((reachability_upper_limit-reachability_lower_limit)*((float)rand()/RAND_MAX) + reachability_lower_limit);

       std::vector<int> items = std::vector<int>(round((3-1)*((float)rand()/RAND_MAX) + 1)); //random size for item vect

       for(int j = 0; j < items.size(); j++)
       {
           if(j > 0)
           {
                int temp = round((3-1)*((float)rand()/RAND_MAX) + 1);
                while(*std::find(items.begin(), items.end(), temp) == temp)
                {
                    temp = round((3-1)*((float)rand()/RAND_MAX) + 1);
                }

                items[j] = temp;
           }
           else //1st iteration
           {
              items[j] = round((3-1)*((float)rand()/RAND_MAX) + 1);
           }

       }//end for

      float x = points[i].x;
      float y = points[i].y;
      float z = points[i].z;

      float weight = 0;
      std::vector<int> quantity = std::vector<int>(items.size()); //quantity dimensions = items dim

      for(int a = 0; a < items.size(); a++)
      {
          switch(items[a])
          {
            case 1:
              quantity[a] = std::round((quantity_upper_limit- quantity_lower_limit)*((float)rand()/RAND_MAX) + quantity_lower_limit);
              weight = weight + 1.5*quantity[a];
              break;
          case 2:
          case 3:
          case 6:
              quantity[a] = std::round((quantity_upper_limit- quantity_lower_limit)*((float)rand()/RAND_MAX) + quantity_lower_limit);
              weight = weight + 1*quantity[a];
              break;
          case 4:
              quantity[a] = std::round((2 - quantity_lower_limit)*((float)rand()/RAND_MAX) + quantity_lower_limit);
              weight = weight + 5*quantity[a];
              break;
          case 5:
              quantity[a] = std::round((1 - quantity_lower_limit)*((float)rand()/RAND_MAX) + quantity_lower_limit);
              weight = weight + 20*quantity[a];
              break;
          default:
//              return 1; //abnormal functioning
              break;

          } //end switch
      } //end for
    /** reachability is taken into account as int number in this part.
        --posible to use ranges in else if conditions instead of switch**/

      switch((int)reachability)
      {
        case 1:
          if(weight >  5)
          {
              items.clear();
              quantity.clear();
              continue;
          }
          break;
      case 2:
        if(weight >  40)
        {
            items.clear();
            quantity.clear();
            continue;
        }
        break;
      case 3:
        if(weight >  40)
        {
            items.clear();
            quantity.clear();
            continue;
        }
        break;
      }

      generated_tasks.push_back(Task(items,x,y,z,urgency,reachability,quantity,std::to_string(i)));

      items.clear();
      quantity.clear();

      i++;

    } //end while

    return generated_tasks;

}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

tuple3v TaskManager::setPriorityTasks(std::vector<Task>& tasks)
{
    tuple3v priorities = tuple3v(); //priorities[1] = urgent, priorities[2] = important, priorities[3] = not_important
    for(std::vector<Task>::iterator i = tasks.begin(); i != tasks.end(); i++)
    {
        if(i->getStatus() != "available") continue;


        switch(i->getUrgency())
        {
            case 1:
            std::get<0>(priorities).push_back(*i);
            break;
            case 2:
            std::get<1>(priorities).push_back(*i);
            break;
            case 3:
            std::get<2>(priorities).push_back(*i);
            break;
            default:
            std::cout << "Non-defined urgency" << std::endl;

        }
    }//end for

    return priorities;
}

//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

 void TaskManager::generateBids(std::vector<Task>& tasks)
 {
     std::vector<Task> urgent_t;
     std::vector<Task> important_t;
     std::vector<Task> not_important_t;

     std::tie(urgent_t, important_t, not_important_t) = this->setPriorityTasks(tasks);
      urgent_bids = this->computeBiddingList(urgent_t, urgent);
      important_bids = this->computeBiddingList(important_t, important);
      not_important_bids = this->computeBiddingList(not_important_t, not_important);
 }
 //______________________________________________________________________________________________________________________
 //______________________________________________________________________________________________________________________

 std::unordered_map<std::string,std::vector<Bid>> TaskManager::computeBiddingList(std::vector<Task>& tasks, Urgency urgency)
 {
     std::unordered_map<std::string,std::vector<Bid>> dict; //bidList as map relating task id and its bids
     if(tasks.size() > 0) //if there are available tasks (do the operations)
     {
         for(std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
         {
             if(it->getStatus() != "available")
                 continue;
             //Get the robots that are able to reach the task as a pointer to the original object;
             std::vector<Robot*> reachable_robots = this->getTaskReachability(*it);
             availRobots.clear();
             //Get the robots with sufficient energy from reachable_robots;
             getDistance(*it,reachable_robots);
             //Get task weight
             float weight = it->getWeight();
             std::vector<float> payloads = getRobotsPayload(reachable_robots);
              float max_available_payload = 0;
             auto l = std::max_element(payloads.begin(),payloads.end());
             if(l != payloads.end())
                 max_available_payload = *l;

             // Determine the robots that will actually bid on the task.
             determineFinalAvailRobots(reachable_robots, weight);
            if(max_available_payload >= weight && reachable_robots.size() > 0)
            {
                std::vector<Bid> task_bids;
                Point3D task_location = it->getTasklLocation();
                for(std::vector<Robot*>::iterator p = reachable_robots.begin(); p != reachable_robots.end(); p++)
                {
                    Point3D r_loc = (*p)->getLocation();
                    float dist  = euclideanDistance(r_loc.x, task_location.x, r_loc.y, task_location.y, r_loc.z, task_location.z);
                    task_bids.push_back(Bid(*p, dist/(*p)->getSpeed() ) );
                    (*p)->pushBid(it->getID(),dist, urgency);
                }
                std::pair<std::string,std::vector<Bid>> bid_pair(it->getID(),task_bids);
                mykeys.push_back(it->getID());
                dict.insert(bid_pair);
                task_bids.clear();
            }
         } //end for (tasks)
     }

    return dict;
 }
 //______________________________________________________________________________________________________________________
 //______________________________________________________________________________________________________________________

  std::vector<Robot*> TaskManager::getTaskReachability(Task& task)
  {
      Robot* p = 0;
      switch(task.getReachability())
      {
          case 1:
             for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end();i++)
             {
                 if(i->ID == "1" || i->ID == "2" || i->ID == "3")
                 {
                     p = &*i;
                     availRobots.push_back(p);
                 }
             }
          break;
      case 2:
         for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end();i++)
         {
             if(i->ID == "4" || i->ID == "5" || i->ID == "6")
             {
                  p = &*i;
                 availRobots.push_back(p);
             }
         }
          break;
      case 3:
          for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end();i++)
          {
              p = &*i;
              availRobots.push_back(p);
          }
          break;
      default:
          //Non reachable value
          break;
      } //end switch

      int i = 0;

      while(i < availRobots.size())
      {
          if(availRobots[i]->getStatus() != "available")
              availRobots.erase(availRobots.begin()+i);
          else
              i++;
      }

      return this->getAvailableRobots();

  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

    std::vector<Robot*> TaskManager::getAvailableRobots(){return availRobots;}
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  std::vector<Allocat> TaskManager::marketBasedAllocation(Urgency urgency)
  {
      std::vector<Allocat> allocations;
      std::vector<std::string> preferences;
      do
      {
          preferences.clear();
          for (auto& x: robots)
          {
              preferences.push_back(x.getBestBid(urgency));
          }

          std::unordered_map<std::string, std::vector<int> > repeated;
          for (int i = 0; i < preferences.size(); i++)
          { //ckeck for problems here when testing
              auto result = repeated.insert(std::pair<std::string, std::vector<int>>(preferences[i], std::vector<int>(1,i)));
              if (result.second == false)
                  result.first->second.push_back(i);
          }

          repeated.erase("0");

          for (auto & elem : repeated)
          {
              // If frequency count is greater than 1 then its a duplicate element
              if (elem.second.size() == 1 )
              {
                  //allocate
                  allocations.push_back(this->createAllocation(elem.first, elem.second[0], urgency));
                  this->eraseBidFromRobots(elem.first,urgency);

              }
              else if(elem.second.size() > 1)
              {
                  std::vector<int> conflict = elem.second;
                  std::vector<int>::iterator winner =conflict.begin();
                  for(std::vector<int>::iterator i =conflict.begin(); i != conflict.end(); i++)
                  {
                      float bid_one = robots[*winner].getBidValue(elem.first, urgency);
                      float bid_two = robots[*i].getBidValue(elem.first, urgency);
                      if(bid_two < bid_one)
                          winner = i;
                  }
                  //allocate_winner
                   allocations.push_back(this->createAllocation(elem.first, *winner, urgency));
                   this->eraseBidFromRobots(elem.first,urgency);
              }
          } //end for
      }//end do
      while(!std::none_of(preferences.begin(), preferences.end(), [](std::string s){return std::stof(s);}));
      //none of returns true when all UnaryPredicate return 0

      return allocations;

  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  Allocat TaskManager::createAllocation(std::string task_id, int robot_idx, Urgency urgency)
  {
    Allocat ret;
    ret.first = task_id;
    ret.second = robots[robot_idx].getID();

    //erase task from pending & set robot as "comitted"
    if(urgency == urgent)
    {
      urgent_bids.erase(urgent_bids.find(task_id));
    }
    else if(urgency == important)
    {
        important_bids.erase(important_bids.find(task_id));
    }
    else if(urgency == not_important)
    {
        not_important_bids.erase(not_important_bids.find(task_id));
    }

    robots[robot_idx].setStatus("comitted");

    return ret;

  }

  void TaskManager::eraseBidFromRobots(std::string id, Urgency urgency)
  {
    for(std::vector<Robot>::iterator i = robots.begin(); i != robots.end(); i++)
        i->removeBid(id, urgency);
  }

  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  std::vector<Allocat> TaskManager::marketBasedAllocation()
  {
    //we allocate iteratively
    std::vector<Allocat> urgent_alloc = this->marketBasedAllocation(urgent);
    std::vector<Allocat> important_alloc = this->marketBasedAllocation(important);
    std::vector<Allocat> not_important_alloc = this->marketBasedAllocation(not_important);

    //move elements
    urgent_alloc.insert(urgent_alloc.end(),std::make_move_iterator(important_alloc.begin()),std::make_move_iterator(important_alloc.end()));
    urgent_alloc.insert(urgent_alloc.end(),std::make_move_iterator(not_important_alloc.begin()),std::make_move_iterator(not_important_alloc.end()));

    urgent_bids.clear();
    important_bids.clear();
    not_important_bids.clear();

    return urgent_alloc;

  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  void TaskManager::eraseAllocatedTasks(std::vector<Task>& tasks, std::vector<Allocat>& allocation)
  {
        for(std::vector<Allocat>::iterator i = allocation.begin(); i != allocation.end(); i++)
        {
            std::string id = i->first;
            for(std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
            {
                if(it->getID() == id)
                {
                    tasks.erase(it);
                    break;
                }
            }
        }
  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  std::vector<Task> TaskManager::extractAllocatedTasks(std::vector<Task>& tasks, std::vector<Allocat>& allocation)
  {
      std::vector<Task> allocated_tasks;
      for(std::vector<Allocat>::iterator i = allocation.begin(); i != allocation.end(); i++)
      {
          std::string id = i->first;
          for(std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
          {
              if(it->getID() == id)
              {
                  allocated_tasks.push_back(*it);
                  tasks.erase(it);
                  break;
              }
          }
      }

      return allocated_tasks;
  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  float TaskManager::roundPoints(std::vector<Task> &tasks, std::vector<Allocat> &allocation)
  {
      float points = 0;
      for(std::vector<Allocat>::iterator i = allocation.begin(); i != allocation.end(); i++)
      {
          std::string idt = i->first;
          std::string idr = i->second;
          Point3D tloc,rloc;

          for(std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
          {
              if(it->getID() == idt)
              {
                  tloc = it->getTasklLocation();
                  break;
              }
          }
          for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
          {
              if(it->getID() == idr)
              {
                  rloc = it->getLocation();
                  points += euclideanDistance(rloc.x,tloc.x,rloc.y,tloc.y,rloc.z,tloc.z)/(it->getSpeed());
                  break;
              }
          }

      } //end alloc iteration

      return points;
  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

  float TaskManager::simulateRoundResults(std::vector<Task>& tasks, std::vector<Allocat>& allocation)
  {
      float charging_time = 0;

      if(allocation.size() > 0)
      {
          for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
          {
              if(it->getStatus() == "comitted")
              {
                  Point3D tloc,rloc;
                  float bid = 0;
                  std::string task_id = "non-sense";
                  //
                  rloc = it->getLocation();
                  //search the allocation to get the task corresponding to this robot
                  for(std::vector<Allocat>::iterator i = allocation.begin(); i != allocation.end(); i++)
                  {
                      if(i->second == it->getID())
                      {
                          task_id = i->first;
                          break;
                      }
                  }
                  //search for the task to get the location
                  for(std::vector<Task>::iterator i = tasks.begin(); i != tasks.end(); i++)
                  {
                      if(i->getID() == task_id)
                      {
                          tloc = i->getTasklLocation();
                          break;
                      }
                  }

                  bid = euclideanDistance(rloc.x,tloc.x,rloc.y,tloc.y,rloc.z,tloc.z)/(it->getSpeed());
                  it->setStatus("available");
                  it->discharge(bid*2);
                  if(bid > charging_time)
                  {
                      charging_time = bid;
                  }
              } //end if comitted
              else if(it->getStatus() == "available")
              {
                  it->clearBids(); // in the commited they were erased before
                  it->setStatus("charging");
              }
          }// end for
          //once we know round time we are going to charge robots that where waiting
          charging_time*=2; //go and come back
          for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
          {
            if(it->getStatus() == "charging")
            {
                it->charge(charging_time);
                it->setStatus("available");
            }
          }
      } //end if allocation

      else //we charge all robots 1 min -->possibly changed at a future point
      {
          charging_time = 60;
          for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
          {
            if(it->getStatus() == "available")
            {
                it->charge(charging_time); //charge 1 min
            }
          }
      }

      return charging_time;

  }
  //______________________________________________________________________________________________________________________
  //______________________________________________________________________________________________________________________

void TaskManager::printBids(std::ostream &out)
{
    char t = '\t';

    out << "URGENT BIDS" << std::endl;

    for(auto& i: urgent_bids)
    {
        out << i.first << " | ";
        for(std::vector<Bid>::iterator it = i.second.begin(); it != i.second.end(); it++)
        {
            out << (it->getRobot())->getID() <<'-' << it->getBid() << t;
        }
        out << std::endl;
    }

    out << "IMPORTANT BIDS" << std::endl;

    for(auto& i: important_bids)
    {
        out << i.first << " | ";
        for(std::vector<Bid>::iterator it = i.second.begin(); it != i.second.end(); it++)
        {
            out << (it->getRobot())->getID() <<'-' << it->getBid() << t;
        }
        out << std::endl;
    }

    out << "NOT IMPORTANT BIDS" << std::endl;

    for(auto& i: not_important_bids)
    {
        out << i.first << " | ";
        for(std::vector<Bid>::iterator it = i.second.begin(); it != i.second.end(); it++)
        {
            out << (it->getRobot())->getID() <<'-' << it->getBid() << t;
        }
        out << std::endl;
    }
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::printAllocation(std::vector<Allocat>& allocation, std::ostream& out)
{
    char t = '\t';
    out << "ALLOCATION" << std::endl << "[task-id|robot-id]: ";
    for(auto& i: allocation)
    {
        out << '[' << i.first << '|' << i.second << ']' << t;
    }
    out << std::endl;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::printSwarmStatus( std::ostream& out)
{
    char t = '\t';
    out << "SWARM STATUS" << std::endl;
    for(std::vector<Robot>::iterator it = robots.begin(); it != robots.end(); it++)
    {
        out << "Robot:" << it->getID() << t << "status:" << it->getStatus();
        out << t << "energy level:" << it->getEnergy() << std::endl;
    }
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<Allocat> TaskManager::customAllocation()
{
    std::random_shuffle ( robots.begin(), robots.end() );
    return this->marketBasedAllocation();
}

//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<Allocat> TaskManager::allocateTasks(allocType type, std::ostream& out)
{
    if(type == NAIVE_MARKET)
    {
        std::vector<Allocat> allocation = this->marketBasedAllocation();
        return allocation;
    }
    if(type == CUSTOM_ALLOC)
    {
        std::vector<Allocat> allocation = this->customAllocation();
        return allocation;
    }
    else
    {
        out << "FAILURE FINDING TYPE" << std::endl;
        return std::vector<Allocat>(0);
    }
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::simulateMission(std::ostream& out)
{
    out << "_______MISSION SIMULATION_______" << std::endl;
    if(this->loadRobots("/home/marcos/MRTA_algorithm_cpp/robots2.txt") == false)
        out << std::endl << "IMPOSSIBLE TO LOAD ROBOTS: check file path" << std::endl;

    srand(time(NULL));
    std::vector<Task> victims = this->generateSampleTasks(std::round((std::rand()/(RAND_MAX + 1.0))*10));

    out << "Generated Tasks:" << victims.size() << std::endl;
    float mission_distance = 0;
    for(auto& t : victims)
    {
        Point3D loc = t.getTasklLocation();
        mission_distance += euclideanDistance(loc.x,0,loc.y,0,loc.z,0);
    }
    int waves = 1;
    float mission_points = 0;
    float mission_time = 0;

    while(victims.size() > 0 && waves < 100)
    {
        out << "__Wave " << waves << "__" << std::endl;
        this->generateBids(victims);
        out << "Bids inserted" << std::endl;
        this->printBids(out);

        std::vector<Allocat> allocation = this->marketBasedAllocation();
        out << "Allocation done" << std::endl;
        out << "size of tasks: " << victims.size() << " size of Allocation: " << allocation.size() << std::endl;
        this->printAllocation(allocation, out);

        std::vector<Task> taskallocated = this->extractAllocatedTasks(victims,allocation);
        float wave_points = this->roundPoints(taskallocated, allocation);
        mission_points += wave_points;
        out << "Wave Points: " << wave_points << std::endl;
        float wave_time = this->simulateRoundResults(taskallocated, allocation);
        mission_time += wave_time;
        out << "Wave Duration: " << wave_time<< std::endl;
        this->printSwarmStatus(out);
        out << "Pending tasks: " << victims.size() << std::endl << std::endl;
        waves++;
    }

    out << std::endl << "MISSION POINTS: " << mission_points << '\t' << " MISSION TIME: " << mission_time;
    out << '\t' << "AV.SPEED: " << (mission_distance*2)/mission_time << std::endl;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::simulateMission(allocType type, std::ostream& out)
{
    out << "_______MISSION SIMULATION_______" << std::endl;
    if(this->loadRobots("/home/marcos/MRTA_algorithm_cpp/robots2.txt") == false)
        out << std::endl << "IMPOSSIBLE TO LOAD ROBOTS: check file path" << std::endl;

    srand(time(NULL));
    std::vector<Task> victims = this->generateSampleTasks(std::round((std::rand()/(RAND_MAX + 1.0))*10));

    out << "Generated Tasks:" << victims.size() << std::endl;
    float mission_distance = 0;
    for(auto& t : victims)
    {
        Point3D loc = t.getTasklLocation();
        mission_distance += euclideanDistance(loc.x,0,loc.y,0,loc.z,0);
    }
    int waves = 1;
    float mission_points = 0;
    float mission_time = 0;

    while(victims.size() > 0 && waves < 100)
    {
        out << "__Wave " << waves << "__" << std::endl;
        this->generateBids(victims);
        out << "Bids inserted" << std::endl;
        this->printBids(out);

        std::vector<Allocat> allocation = this->allocateTasks(type, out);
        out << "Allocation done" << std::endl;
        out << "size of tasks: " << victims.size() << " size of Allocation: " << allocation.size() << std::endl;
        this->printAllocation(allocation, out);

        std::vector<Task> taskallocated = this->extractAllocatedTasks(victims,allocation);
        float wave_points = this->roundPoints(taskallocated, allocation);
        mission_points += wave_points;
        out << "Wave Points: " << wave_points << std::endl;
        float wave_time = this->simulateRoundResults(taskallocated, allocation);
        mission_time += wave_time;
        out << "Wave Duration: " << wave_time<< std::endl;
        this->printSwarmStatus(out);
        out << "Pending tasks: " << victims.size() << std::endl << std::endl;
        waves++;
    }

    out << std::endl << "MISSION POINTS: " << mission_points << '\t' << " MISSION TIME: " << mission_time;
    out << '\t' << "AV.SPEED: " << (mission_distance*2)/mission_time << std::endl;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

void TaskManager::simulateMission(std::vector<Task> victims, allocType type, std::ostream& out)
{
    //you need to previously load robots and generate tasks, this is to compare methods
    out << "Generated Tasks:" << victims.size() << std::endl;
    float mission_distance = 0;
    for(auto& t : victims)
    {
        Point3D loc = t.getTasklLocation();
        mission_distance += euclideanDistance(loc.x,0,loc.y,0,loc.z,0);
    }
    int waves = 1;
    float mission_points = 0;
    float mission_time = 0;
    out << "Allocation type:" << type << std::endl;
    while(victims.size() > 0 && waves < 100)
    {
        out << "__Wave " << waves << "__" << std::endl;
        this->generateBids(victims);
        out << "Bids inserted" << std::endl;
        this->printBids(out);
        std::vector<Allocat> allocation = this->allocateTasks(type, out);
        out << "Allocation done" << std::endl;
        out << "size of tasks: " << victims.size() << " size of Allocation: " << allocation.size() << std::endl;
        this->printAllocation(allocation, out);

        std::vector<Task> taskallocated = this->extractAllocatedTasks(victims,allocation);
        float wave_points = this->roundPoints(taskallocated, allocation);
        mission_points += wave_points;
        out << "Wave Points: " << wave_points << std::endl;
        float wave_time = this->simulateRoundResults(taskallocated, allocation);
        mission_time += wave_time;
        out << "Wave Duration: " << wave_time<< std::endl;
        this->printSwarmStatus(out);
        out << "Pending tasks: " << victims.size() << std::endl << std::endl;
        waves++;
    }

    out << std::endl << "MISSION POINTS: " << mission_points << '\t' << " MISSION TIME: " << mission_time;
    out << '\t' << "AV.SPEED: " << (mission_distance*2)/mission_time << std::endl;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<std::vector<Allocat>> TaskManager::completeMission(std::vector<Task> victims, allocType type)
{
    std::vector<std::vector<Allocat>> mision_plan;
    //you need to previously load robots and generate tasks, this is to compare methods
    float mission_distance = 0;
    for(auto& t : victims)
    {
        Point3D loc = t.getTasklLocation();
        mission_distance += euclideanDistance(loc.x,0,loc.y,0,loc.z,0);
    }
    int waves = 1;
    float mission_points = 0;
    float mission_time = 0;

    while(victims.size() > 0 && waves < 100)
    {
        this->generateBids(victims);
        std::ofstream nul; //dump content
        std::vector<Allocat> allocation = this->allocateTasks(type, nul);
        mision_plan.push_back(allocation);

        std::vector<Task> taskallocated = this->extractAllocatedTasks(victims,allocation);
        float wave_points = this->roundPoints(taskallocated, allocation);
        mission_points += wave_points;
        float wave_time = this->simulateRoundResults(taskallocated, allocation);
        mission_time += wave_time;
        waves++;
    }
    //add a final vector with a summary information
    std::vector<Allocat> estimation;
    Allocat dis_time, point_iter;
    dis_time.first = std::to_string(mission_distance*2);
    dis_time.second = std::to_string(mission_time);
    estimation.push_back(dis_time);

    point_iter.first = std::to_string(mission_points);
    point_iter.second = std::to_string(waves-1);
    estimation.push_back(point_iter);

    mision_plan.push_back(estimation);

    return mision_plan;
}
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________

std::vector<std::vector<Allocat>> TaskManager::completeMission(std::vector<Task> victims, allocType type, std::ostream& out)
{
    std::vector<std::vector<Allocat>> mision_plan;
    //you need to previously load robots and generate tasks, this is to compare methods
    out << "Generated Tasks:" << victims.size() << std::endl;
    float mission_distance = 0;
    for(auto& t : victims)
    {
        Point3D loc = t.getTasklLocation();
        mission_distance += euclideanDistance(loc.x,0,loc.y,0,loc.z,0);
    }
    int waves = 1;
    float mission_points = 0;
    float mission_time = 0;
    out << "Allocation type:" << type << std::endl;
    while(victims.size() > 0 && waves < 100)
    {
        out << "__Wave " << waves << "__" << std::endl;
        this->generateBids(victims);
        out << "Bids inserted" << std::endl;
        this->printBids(out);
        std::vector<Allocat> allocation = this->allocateTasks(type, out);
        out << "Allocation done" << std::endl;
        mision_plan.push_back(allocation);
        out << "size of tasks: " << victims.size() << " size of Allocation: " << allocation.size() << std::endl;
        this->printAllocation(allocation, out);

        std::vector<Task> taskallocated = this->extractAllocatedTasks(victims,allocation);
        float wave_points = this->roundPoints(taskallocated, allocation);
        mission_points += wave_points;
        out << "Wave Points: " << wave_points << std::endl;
        float wave_time = this->simulateRoundResults(taskallocated, allocation);
        mission_time += wave_time;
        out << "Wave Duration: " << wave_time<< std::endl;
        this->printSwarmStatus(out);
        out << "Pending tasks: " << victims.size() << std::endl << std::endl;
        waves++;
    }

    out << std::endl << "MISSION POINTS: " << mission_points << '\t' << " MISSION TIME: " << mission_time;
    out << '\t' << "AV.SPEED: " << (mission_distance*2)/mission_time << std::endl;
    //add a final vector with a summary information
    std::vector<Allocat> estimation;
    Allocat dis_time, point_iter;
    dis_time.first = std::to_string(mission_distance*2);
    dis_time.second = std::to_string(mission_time);
    estimation.push_back(dis_time);

    point_iter.first = std::to_string(mission_points);
    point_iter.second = std::to_string(waves-1);
    estimation.push_back(point_iter);

    mision_plan.push_back(estimation);

    return mision_plan;
}
