/** ***************************************************************************************************
 * Example showing the usual flow using  the high level methods of the library
 * + save the mission points generated for further analysis
 * ***************************************************************************************************
 */
#include <iostream>

#include "taskmanager.h"

using namespace std;

int main()
{
    std::ofstream debug, mission_points;
    
    // Debug stream will be passed to the task manager in order to collect relevant debug info
    debug.open("Documents/C++/MRTAlib/resources/mission_performance_print.txt", std::ios::out|std::ios::app);
    if(!debug)
    {
        return -1;
    }
    
    //Mission points is used to store task locations for future review
    mission_points.open("/Documents/C++/MRTAlib/resources/points.txt", std::ios::out|std::ios::app);
    if(!mission_points)
    {
        return -1;
    }
    
    //Create the Manager, load robot specs and generete sample random tasks
    TaskManager marketBasedManager = TaskManager();
    marketBasedManager.loadRobots("Documents/C++/MRTAlib/resources/robots2.txt");
    std::vector<Task> t = marketBasedManager.generateSampleTasks(30); //since its the inner, robots must be loaded first

    
    //Pass task locations to mission points txt
    for(std::vector<Task>::iterator i = t.begin(); i != t.end(); i++)
    {
        Point3D p = i->getTasklLocation();
        mission_points << p.x <<',' << p.y <<',' << p.z << std::endl;
    }
    mission_points.close();

    //Complete the tasks and extract the performance from output
    std::vector<std::vector<Allocat>> allocation = marketBasedManager.completeMission(t,NAIVE_MARKET, std::cout);
    
    float emd = std::stof((allocation[allocation.size()-1])[0].first);
    float emt = std::stof((allocation[allocation.size()-1])[0].second);
    
    debug << emd/emt << std::endl;
    debug.close();
    
    return 0;
}
