/***********************************************************************
 * Velocity c++ file
 * Author:
 *    Nathaniel Mason
 * Summary:
 *    This file handles the angles, speed, and velocity for our entire
 *    program.
 ************************************************************************/

#include "velocity.h"


Velocity::Velocity(float pdx, float pdy, int pangle, int pspeed)
{
   //Velocity
   //setDx();
   //setDy();
   dx = 0.0;
   dy = 0.0;
   this->speed = 1;
   
}

/*******************************************
 * COPY CONSTRUTCTOR
 *  Copies relevant data to a copy object
 *   position (x  and y )
 *   velocity (dx and dy)
 *******************************************/
Velocity::Velocity(Velocity & velocityOrig)
{
   //Copy velocity
   //setDx(velocityOrig.getDx());
   //setDy(velocityOrig.getDy());
}

/***********************************************
 * DESTRUCTOR
 ***********************************************/
Velocity::~Velocity()
{
   
}


/************************************************
 * SET DX
 *  Mutator function for member 'dx'
 *    INPUT:  dx - the new dx
 ************************************************/

void Velocity::setVelocity(float angle, float speed)
{
   float rad = (angle - 90) * 3.1415 / 180;
   this->dx = speed *  -cos(rad);
   this->dy = speed *  -sin(rad);
   //angle = angle * 180 / 3.1415;
   
   //angle = angle * 180 / 3.1415;
   //this->dx = speed * sin(angle);
   //angle = angle * 3.1415 / 180;
  
}


/*************************************************
 * 
 *************************************************/
void Velocity::setAngle(float newAngle)
{
   if (newAngle < 0)
   newAngle += 360;
   if (newAngle > 360)
   newAngle -= 360;
   angle = newAngle;
  
}

/*************************************************
 * setSpeed sets the speed for objects
 *************************************************/
void Velocity::setSpeed(float newSpeed)
{
   this->speed = newSpeed;
}

/*************************************************
 * handles when objects hit each other
 *************************************************/
float Velocity::minimumDistance(Velocity a, Velocity  b)
{
   
   float distanceSquared;
   float minDist;
   float slice = 1 / (max(max(a.getDx(), a.getDy()   ),
                         max(b.getDx(), b.getDy())  ));
   for (float percent = 0; percent <= 100; slice++)
   {
      float distaceSquared = ((a.getX() + a.getDx() * percent) - pow((b.getX() + b.getDx() * percent),2)) +
         ((a.getY() + a.getDy() * percent) - pow((b.getY() + b.getDy() * percent),2) );
      float minDist = min(distanceSquared, minDist);
   }

   return sqrt(minDist);
}

/*************************************************
 * adds velocity to superman
 *************************************************/
void Velocity::operator += (Velocity & rhs)
{
   this->dx += rhs.dx;
   this->dy += rhs.dy;
}

/*************************************************
 * adds velocity to superman
 *************************************************/
Velocity Velocity::operator + (Velocity & v)
{
   this->dy = this->dy + v.getDy();
   this->dx = this->dx + v.getDx();
}

/*************************************************
 * subtracts velocity from superman
 *************************************************/
Velocity Velocity::operator - (Velocity & v)
{
   this->dy = this->dy - v.getDy();
   this->dx = this->dx - v.getDx();
}

