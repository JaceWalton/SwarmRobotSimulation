/****************************************
 * Header file for the bots. Each has a 
 * Point, a Velocity and, a Behavior. 
 * Each bots behavior should be dynamic
 * and changeable mutch like Velocity
 ***************************************/
#ifndef _NEURON_            
#define _NEURON_
#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "velocity.h"
using namespace std;

#define NEURON_BEHAVIORS 5
#define NUM_WAYPTS 90

class Neuron
{                                    
  public:
   Neuron();
   //calls a function to draw bots on the screen
   void draw();    
   // For User send commands to each bot 
   void interact(const Interface *pUI, void *p);
   // just moves the bot one step forward
   void increment();
   
   //getters
   Point getPt()                         { return pt;          }
   Velocity getVel()                     { return v;           }
   float    getX()                       { return pt.getX();   }
   float    getY()                       { return pt.getY();   }   
   float    getAngle()                   { return v.getAngle();}
   float    getSpeed()                   { return v.getSpeed();}
   bool     getMutex()                   { return mutex;       }
   // setters
   void  setAngle(float theta)           { v.setAngle(theta);  }
   void  setSpeed(float speed)           { v.setSpeed(speed);  }
   void  setMutex(bool mutex)            { this->mutex = mutex;}
   void  setBehavior(int newBH)          { iBehavior = newBH;  }
   //increment the angel up or down
   void  addAngle(float deltaAng)        { v.addAngle(deltaAng);}
   void  addSpeed(float spd)             { v.addSpeed(spd);     }
   int iBehavior;
  protected:
   Point pt;
   Velocity v;
   Point *wayPt;
   int wayI;
   bool mutex;
   // Dynamicaly alocated behavoirs
   // bot can switch to differant behaviors based its state
   // (where it is and what it is doing)
   // Behvior *behavior;

};
#endif //_NEURON_




