/** ***************************************************************************************************
* Example showing the use of single operations (mid-level methods) of the library
* + aproximate measurement of process time
* ***************************************************************************************************
*/
#include <iostream>
#include <fstream>
#include <chrono>

#include "taskmanager.h"

int main(int argc, char* argv[])
{

    std::ofstream debug;
    debug.open("Documents/C++/MRTAlib/resources/mono_times_8.txt", std::ios::out|std::ios::app);

   //Instantiate the object from TaskManager and load robots
   TaskManager marketBasedManager = TaskManager();
    marketBasedManager.loadRobots("Documents/C++/MRTAlib/resources/robots.txt");
    
    //Generate sample tasks from the inner random generator
    std::vector<Task> t = marketBasedManager.generateSampleTasks(8);
    
    //Generate the bidding lists for tasks in t
    marketBasedManager.generateBids(t);
    
    //Create time variables for the measurement of single round allocation time
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    
    //Allocate one single round
    marketBasedManager.marketBasedAllocation();
    
    //Stop time measurement and save & display results
    end = std::chrono::steady_clock::now();
    debug << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << std::endl;
    std::cout << std::endl << "Duration in usecs: " << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count();
    std::cout << std::endl;
    debug.close();

    return 0;
}
