#include "bid.h"

Bid::Bid(Robot &ow, float b)
{
    owner = &ow;
    bid = b;
}
Bid::Bid(Robot* ow, float b)
{
    owner = ow;
    bid = b;
}
Robot* Bid::getRobot(){return owner;}
Robot Bid::getRobotCopy(){return *owner;}
float Bid::getBid(){return bid;}
