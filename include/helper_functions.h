/**
 * Functions from non-OO implementation
 * they help erasing robots non-capable of performing some tasks
 * as part of the bid dict generation
 */
#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>      // std::istringstream
#include <fstream>
#include <tuple>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "task.h"
#include "robot.h"
#include "allocation.h"

inline float euclideanDistance(float x1, float x2, float y1, float y2, float z1, float z2)
{
  return (float) sqrt(pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2));
}

void getDistance(Task& task,  std::vector<Robot*>& availRobots);
std::vector<float> getRobotsPayload(std::vector<Robot*>& availRobots);
void determineFinalAvailRobots(std::vector<Robot*>& robots, float weight);
std::vector<Task> loadTasks(std::string filepath);
std::vector<Task>::iterator findTask(std::vector<Task> &taskvector, std::string task_id);

#endif // HELPER_FUNCTIONS_H
