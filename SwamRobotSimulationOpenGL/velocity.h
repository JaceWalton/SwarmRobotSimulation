#ifndef _Velocity_
#define _Velocity_
#include <cmath>
#include <algorithm>
#include "point.h"
using namespace std;

#define PI 3.1415

class Velocity : public Point
{

  public:
   Velocity(float dx = 0, float dy = 0, int angle = 0, int speed = 0);
   Velocity(Velocity & velocityOrig);
   ~Velocity();
   const float getDx()  const { return dx; };
   const float getDy()  const { return dy; };
   const float getAngle() const {return angle;};
   const int getSpeed() const {return speed;};
   // void setX(int a)         { x = a;      }       
   // void setY(int a)         { y = a;      }
   void setVelocity(float angle, float speed);
   //void setDy();
   void setAngle(float newAngle);
   void setSpeed(float speed);
   float setDx(float dx)      { dx = dx; }
   float setDy(float dy)      { dy = dy; }
   void addSpeed(int newSpd)  {speed += newSpd;}
   void addAngle(float newAng)  {setAngle(angle + newAng);}
   float minimumDistance(Velocity a, Velocity b);
   void operator  += (Velocity & rhs);
   Velocity operator + (Velocity & v);
   Velocity operator - (Velocity & v);
  
   
  private:
   Point point;
   float dx;
   float dy;
   // angles were once ints' but are floats to faciltiate in PID with tracatio n of ints PID would converge to +/- 1 consider ints' for small MCUs
   float angle;
   float speed;
   // int x;
   //int y;
};       

#endif

