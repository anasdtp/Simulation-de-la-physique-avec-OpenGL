#include "animation.h"


Animation::Animation(reel ph, reel th, Vector accel, Vector speed, Point p)
{
    // Constructor
    // Initialization
    phi = ph;
    theta = th;
    acc = accel;
    spd = speed;
    pos = p;
}

bool operator==(const Color &c1, const Color &c2)
{
    
    if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b){
        return true;
    }
    return false;
}
