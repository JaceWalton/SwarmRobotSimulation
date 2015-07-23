/****************************************
 * Header file for the bots. Each has a 
 * Point, a Velocity and, a Behavior. 
 * Each bots behavior should be dynamic
 * and changeable mutch like Velocity
 ***************************************/
#ifndef _DISTSENSE_            
#define _DISTSENSE_
#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "velocity.h"
using namespace std;


class DistSense : public Neuron
{                                    
  public:
   DistSense();
   //draws sensor skirt
   void draw();    
   // For User send commands to each bot 
   int asdf()
   {
      int dist =calcDistBetwixt; 
      if ((dist < 200) && (dist() > 20)
         {
            int angle = calcAngleToTarget(pt,otherPt);
         }
   }


   

  protected:


};
#endif //_DISTSENSE_




