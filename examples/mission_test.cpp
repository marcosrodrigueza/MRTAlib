/** ***************************************************************************************************
* Example showing the usual flow using  the high level methods of the library
* + aproximate measurement of process time
* ***************************************************************************************************
*/
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#include "taskmanager.h"

using namespace std;

int main()
{
    std::ofstream debug;
    debug.open("Documents/C++/MRTAlib/resources/mission_times_30.txt", std::ios::out|std::ios::app);
    
    //Instantiate the object,load the robots, and use the inner random generator to create tasks
    TaskManager marketBasedManager = TaskManager();
    marketBasedManager.loadRobots("Documents/C++/MRTAlib/resources/robots.txt");
    std::vector<Task> t = marketBasedManager.generateSampleTasks(30);
    
    //Define time measurement variables and start counting
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    
    //Perform the allocation (without capturing the result)
    marketBasedManager.completeMission(t,NAIVE_MARKET, std::cout);
    
    //Stop counting and save & display time results
    end = std::chrono::steady_clock::now();
    debug << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << std::endl;
    
    std::cout << std::endl << "Duration in usecs: " << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count();
    
    std::cout << std::endl;
    
    debug.close();
    return 0;
}
