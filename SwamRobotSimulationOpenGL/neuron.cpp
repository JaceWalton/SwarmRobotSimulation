/***********************************************************************
* neuron c++ file
* Author:
*    Jace Walton
* Summary: 
*  This File is the base code for each   
*    
*    
************************************************************************/

#include "neuron.h"
#include "ganglion.h" // to have BEHAVIORS Macro

/****************************************
 * default constuctor for neuron
 * this sets some variables when a superman object is made
 ***************************************/
Neuron::Neuron()
{
   //iBehavior = PUSHOBJTOGOAL;
   iBehavior = CONVTOGOALWCAUTION;
   // iBehavior = RONDEZVOUS;
   // iBehavior = CONVTOGOAL;
   // iBehavior = CONVCLOSEST;
   //iBehavior = BEHAVIORS+1;
   mutex = 1;
   wayI = 0;
   wayPt = new Point[NUM_WAYPTS];
   //Dynamicaly alocatted nuber of behaviors
   //behavior = new Behavior[NEURON_BEHAVIORS];
   pt.setWrap(true);
   // appear at any random spot
   pt.setX(random(pt.getXMin(), pt.getXMax()));
   pt.setY(random(pt.getYMin(), pt.getYMax()));
   // with any random angle
   v.setAngle(random(0, 360));
}

/****************************************
 * how superman interacts with the other 
 * objects
 ***************************************/
void Neuron::draw()
{
   drawBot(pt, v.getAngle());
   // code for creating wayPts
   
   //wayPt[wayI].setX(pt.getX());
   //wayPt[wayI].setY(pt.getY());
   wayPt[wayI]= pt;
   wayI++;  
   if (wayI >= NUM_WAYPTS)
      wayI = 0;
   for (int i = 0; i < NUM_WAYPTS; i++)
   {
      drawDot(wayPt[i]);
   }

}

/****************************************
 * how superman interacts with the other 
 * objects
 ***************************************/
void Neuron::interact(const Interface *pUI, void *p)
{
   //directions buttons are state buttons
   //space is event button IE holding space bar
   //does not produce multiple events

   int asdf;


   if (pUI->isRight())
   {
      v.addAngle(-5);
   }
   
   if (pUI->isLeft())
   {
      v.addAngle( 5);
   }
   
   if (pUI->isUp())
   {
      v.addSpeed(1);
   }

   if (pUI->isDown())   
   {
      v.addSpeed(-1);
   }

   // bot jumps to random position and orientation
   if (pUI->isSpace())
   {
      pt.setX(pt.getX() + random(pt.getXMin(), pt.getXMax()));
      pt.setY(pt.getY() + random(pt.getYMin(), pt.getYMax()));
      v.setAngle(random(0, 360));
   }
}

/****************************************
 * increment adds a minute change to postion
 * and then sets to velocity
 ***************************************/
void Neuron::increment()
{
   v.setVelocity(v.getAngle(), v.getSpeed());
   pt.addX(v.getDx());
   pt.addY(v.getDy());

}

