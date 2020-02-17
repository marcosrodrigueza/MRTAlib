#ifndef BID_H
#define BID_H

#include "robot.h"

class Bid
{
    private:
    Robot* owner;
    float bid;

    public:
    Bid(Robot& ow,float b);
    Bid(Robot* ow, float b);
    Robot* getRobot();
    Robot getRobotCopy();
    float getBid();
};

#endif // BID_H
