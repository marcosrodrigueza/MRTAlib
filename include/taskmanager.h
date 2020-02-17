#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <utility>      // std::pair
#include "task.h"
#include "robot.h"
#include "allocation.h"
#include "bid.h"
#include "custom_types.h"
#include "helper_functions.h"

///DEFINITIONS FOR CLEARNESS
typedef std::tuple<std::vector<Task>,std::vector<Task>,std::vector<Task>> tuple3v;
typedef std::pair<std::string, std::string> Allocat; //task-id|robot-id
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The TaskManager class
/// Central object in charge of managing Allocation
class TaskManager
{
    private:
    std::vector<Robot> robots;
    std::vector<std::string> mykeys;
    std::unordered_map<std::string,std::vector<Bid>> urgent_bids;
    std::unordered_map<std::string,std::vector<Bid>> important_bids;
    std::unordered_map<std::string,std::vector<Bid>> not_important_bids;
    std::vector<Robot*> availRobots;//temporal

    // private methods (Low-level: not accesible)
    tuple3v setPriorityTasks(std::vector<Task>& tasks);
    std::unordered_map<std::string,std::vector<Bid>> computeBiddingList(std::vector<Task>& tasks, Urgency urgency);
    std::vector<Robot*> getTaskReachability(Task& task);
    std::vector<Allocat> marketBasedAllocation(Urgency urgency); //must be called after generating bids
    Allocat createAllocation(std::string task_id, int robot_idx, Urgency urgency);
    void eraseBidFromRobots(std::string id, Urgency urgency);
    std::vector<Robot*> getAvailableRobots();

    public:
    //mid-level interface(public): control of unit operations
    TaskManager();
    TaskManager(std::vector<Robot> swarm);
    void addRobot(const Robot& r);
    bool removeRobot(std::string r);
    bool removeRobot(Robot r);
    bool loadRobots(std::string filepath);
    void respawnRobots();
    std::vector<Task> generateSampleTasks(int n);
    void generateBids(std::vector<Task>& tasks);
    std::vector<Allocat> marketBasedAllocation();
    void eraseAllocatedTasks(std::vector<Task>& tasks, std::vector<Allocat>& allocation);
    std::vector<Task> extractAllocatedTasks(std::vector<Task>& tasks, std::vector<Allocat>& allocation);
    float roundPoints(std::vector<Task>& tasks, std::vector<Allocat>& allocation);
    float simulateRoundResults(std::vector<Task>& tasks, std::vector<Allocat>& allocation);
    void printBids(std::ostream& out);
    void printAllocation(std::vector<Allocat>& allocation, std::ostream& out);
    void printSwarmStatus( std::ostream& out);
    //CUSTOM METHOD
    std::vector<Allocat> customAllocation();

    //high-level interface(public):
    std::vector<Allocat> allocateTasks(allocType type, std::ostream& out);
    void simulateMission(std::ostream& out);
    void simulateMission(allocType type, std::ostream& out);
    void simulateMission(std::vector<Task> victims, allocType type, std::ostream& out);
    std::vector<std::vector<Allocat>> completeMission(std::vector<Task> victims, allocType type);
    std::vector<std::vector<Allocat>> completeMission(std::vector<Task> victims, allocType type, std::ostream& out);
};
#endif // TASKMANAGER_H
