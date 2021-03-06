/***********************************************************************
 * Author:
 *    Jace Walton
 * Summary: 
 *    This program inplements the class Neuron where all things are executed
 *    Ganglion handles all behaviors of the robots at a gloabal level.
 *    Meaning that Ganglion is the hive mind that controles and operates
 *    each neuron.
 *    Each behavior is made up of two or more sub-behaviors. Thus by mixing
 *    and matching various sub-behaviors, more complex behaviors can be
 *    achieved, such as; rondezvous is comprized of calcCenterMass and
 *    convToGoal(center) and go to oringin is comprized of convToGoal(origin).
 ************************************************************************/

#include "ganglion.h"
#include <list>
/******************************************
 * Contructor only calls reset thus the simulation can be
 * reset mid execution
 ******************************************/
Ganglion::Ganglion()
{
   reset();
}

/******************************************
 * allocates memory to bots and sets wrapping
 * state of points and sets position of target
 ******************************************/
void Ganglion::reset()
{
   behaviors[CONVCLOSEST]           = CONVCLOSEST;
   behaviors[RONDEZVOUS]            = RONDEZVOUS;
   behaviors[CONVTOGOAL]            = CONVTOGOAL;
   behaviors[GOTOGOAL]              = GOTOGOAL;
   behaviors[CONVTOGOALWCAUTION]    = CONVTOGOALWCAUTION;
   behaviors[PUSHOBJTOGOAL]         = PUSHOBJTOGOAL;
   behaviors[BEHAVIORS]             = BEHAVIORS;
   
   //initially bots have no behavor must be set by user for now
   //idealy the bots would decide their own behavior 
   //iBehavior = BEHAVIORS+1;
   goal.setY(-250);
   neuron = new Neuron[NUM_NEURONS];
   // goal point wraps so it will not go off the screen and get lost...
   goal.setWrap(true);
   // initialize the target
   target.setWrap(true);
   target.setX(random(target.getXMin() / 2, target.getXMax() / 2));
   target.setY(random(target.getYMin() / 2, target.getYMax() / 2));
}

/******************************************
 * draws each Bot with its index ID in its center
 ******************************************/
void Ganglion::draw()
{
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      neuron[i].draw();
      drawNumber(neuron[i].getPt(),i);
   }
}

/******************************************
 * tell all bots to increment at 30FPS
 * FPS is set by OpenGL in uiInteract.cpp/.h
 ******************************************/
void Ganglion::increment()
{
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      neuron[i].increment();
   }
}

/******************************************
 * These if statements are needed to ensure
 * that the Dx and Dy for the target movement
 * is in the correct direction. If not target
 * will move in the wrong direction this fucntion
 * will no be needed in emplimentation becuse
 * objects move by laws of physics not by code...
 ******************************************/
void Ganglion::targetAdvance(int i)
{
   
   //int angle = neuron[i].getAngle();
   int angle = calcAngleToTarget(target, neuron[i].getPt());
   int spd = neuron[i].getSpeed();
   cout << "angle" << angle << endl;

   if ((angle >= 0) && (angle < 90))
   {
      target.addX(-spd*cos(angle*TORADIAN));
      target.addY( spd*sin(angle*TORADIAN));
   }
   if ((angle >= 90 ) && (angle < 180))
   {
      target.addX( spd*cos(angle*TORADIAN));
      target.addY(-spd*sin(angle*TORADIAN));
   }
   if ((angle >= 180 ) && (angle < 270))
   {
      target.addX(-spd*2*cos(angle*TORADIAN));
      target.addY( spd*2*sin(angle*TORADIAN));
   }
   if ((angle >= 270 ) && (angle < 360))
   {
      target.addX( spd*cos(angle*TORADIAN));
      target.addY(-spd*sin(angle*TORADIAN));
   }
}

/******************************************
 * sends command to each bot. When a key press
 * is made a commad is sent to each neruons
 * interact function
 ******************************************/
void Ganglion::interact(const Interface *pUI, void *p)
{
   if (pUI->isSpace())   
   {
      reset();
   }


   // This mess is supposed to be ablt to change to behavior of the bots
   // not quite wroking 
   cin.clear();
   if (pUI->isB())
   {
      int globalBehavior;
     
      cout << "set Behavior for all neurons " << endl
           << CONVCLOSEST << " convClosest" << endl
           << RONDEZVOUS  << " Rondezvous" << endl
           << CONVTOGOAL  << " convToGoal" << endl
           << GOTOGOAL    << " convToGoal" << endl
           << CONVTOGOALWCAUTION << " convToGoalWCaution" << endl
           << PUSHOBJTOGOAL << " pushOjbToGoal" << endl
           << BEHAVIORS   << " No Behavior" << endl;

      cin >> globalBehavior;
      cin.clear();

      for (int i = 0; i < NUM_NEURONS; i++)
      {
         neuron[i].setBehavior(globalBehavior);
         cout << i << endl;
         cout << "behavior" <<neuron[i].iBehavior << endl;
      }
   }
   // Enable user to move the goal around
   // mostly for debugging and changins direction of bots
   for (int i = 0; i < NUM_NEURONS; i++)
   {
     
      if (neuron[i].iBehavior != BEHAVIORS)
      {
         if (pUI->isRight())
         {
            goal.addX(6);
         }
   
         if (pUI->isLeft())
         {
            goal.addX(-6);
         }
   
         if (pUI->isUp())
         {
            goal.addY(6);
         }

         if (pUI->isDown())   
         {
            goal.addY(-6);
         }

      }

      // If behviors-behavior is selected bots are controlled directly by
      // the user
      if (neuron[i].iBehavior == BEHAVIORS)
         for (int i = 0; i < NUM_NEURONS; i++) 
         {
            neuron[i].interact(pUI, p);
         }
   }
}


/******************************************
 * This calls the behavoir functions and
 * impliments high level behavior 
 ******************************************/
void Ganglion::behavior()
{
   //pushObjToGoal();
  
   //convIavgBoth()
   //obstAvoid(origin);
   //convToGoal(goal);
   for (int i = 0; i < NUM_NEURONS; i++) 
   {
      switch(behaviors[neuron[i].iBehavior])
      {
         case CONVCLOSEST:
            convClosest();      
            break;
         case RONDEZVOUS:
            rondezvous();
            break;
         case CONVTOGOAL:
            convToGoal(goal, i);
            break;
         case GOTOGOAL:
            convToGoal(goal, i);
            break;
         case CONVTOGOALWCAUTION:
            convToGoalWCaution(i);
            break;
         case PUSHOBJTOGOAL:
            pushObjToGoal();
            break;
         case BEHAVIORS:
            break;
      }
   }

   //convClosest();
   //rondezvous();
   //convToGoal(origin);
   //lineBetwixtAll();       
   //lineBetwixtClosest(5);
}


float Ganglion::PID(float error, float epk, float eik, float edk)
{
   // this keeps the error withing proper bounds
   if (error < -180)
      error += 360;
   if (error > 180)
      error -= 360;

   // eP contributes to state increasing quickly towards the referacen but
   // on its own will never achive this value.
   float eP = error * epk;
   // eI drives 

   float eI = error * eik;
   // eD is 
   float eD = error * edk;
   float pid = eP + eI + eD;
   return pid;
}


/******************************************
 * Draws a line from each bot to its closest neightbor
 ******************************************/
void Ganglion::convClosest()
{

   //for all bots
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      //reintitialize all the minimum distance
      int jClosest = calcClosest(i);
      float angle = calcAngleToTarget(neuron[jClosest].getPt(),
                                      neuron[i].getPt());
      lineBetwixtClosest(i);

      float error = angle - neuron[i].getAngle();
      float pid = PID(error, 0.01, 0.1, 0.001);
      if (error != 0)
         neuron[i].addAngle(( pid ));
 

      
      // //cout << "bot" << i << "'s dist "      << dist << endl;
      // cout << "bot" << i << "'s new angle " << angle << endl;
      // cout << "bot" << i << "'s angle "  << neuron[i].getAngle() << endl;
      // cout << "bot" << i << "'s error "     << error << endl;
      // if (i == NUM_NEURONS - 1)
      //    cout << endl;
  

   }
}

/******************************************
 * Calculates the center of mass of all bots
 ******************************************/
void Ganglion::calcCenterMass()
{
   float avgX = 0;
   float avgY = 0;

   //for all bots
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      avgX += neuron[i].getX();
      avgY += neuron[i].getY();
   }
   center.setX(avgX/(NUM_NEURONS));
   center.setY(avgY/(NUM_NEURONS));
}


/******************************************
 * Draws a line from each bot to its closest neightbor
 ******************************************/
void Ganglion::rondezvous()
{
   calcCenterMass();
   for (int i = 0; i < NUM_NEURONS; i++)
      convToGoal(center, i);
}

/******************************************
 * Draws a line from each bot to its closest neightbor
 ******************************************/
void Ganglion::convToGoal(Point goal, int i)
{
   drawCircle(goal, 50, 25, 0);
   int distBetwixt;

   float angle = calcAngleToTarget(goal, neuron[i].getPt());


   float error = angle - neuron[i].getAngle();
      
      
   float pid = PID(error, 0.01, 0.1, 0.001);
   if (error != 0)
      neuron[i].addAngle(( pid ));

   distBetwixt = calcDistBetwixt(goal, neuron[i].getPt());
   if (distBetwixt < 50)
   {
      // cout << "bot" << i <<"'s distance" << distBetwixt << endl;
      neuron[i].setSpeed(neuron[i].getSpeed() - 0.001);
   }
   else
      neuron[i].setSpeed(2);

   // cout << "bot" << i <<"'s angle " << neuron[i].getAngle() << endl;
   // if (i == NUM_NEURONS - 1)
   //   cout << endl;
  
      
}


/******************************************
 * Draws a line from each bot to its closest neightbor
 ******************************************/
void Ganglion::obstAvoid(Point obst, int i)
{
   drawCircle(obst, 50, 25, 0);

   int dist = calcDistBetwixt(obst, neuron[i].getPt());
   // if(dist < 50)
   //{
      
   float angle = calcAngleToTarget(obst, neuron[i].getPt());
   angle += 180;
   float error = angle - neuron[i].getAngle();
      
   float pid = PID(error, 0.01, 0.1, 0.001);
   neuron[i].addAngle(( pid ));

      
}

/******************************************
 * convToGoal and obstAvoid
 ******************************************/
void Ganglion::convToGoalWCaution(int i)
{


   int dist = calcDistBetwixt(neuron[i].getPt(), origin);
   if (dist < 100)
      obstAvoid(origin, i);
   else
      convToGoal(goal, i);
                 

}

/******************************************
 * moves bot[i] to temporary goal. 
 ******************************************/
void Ganglion::botToG1(int i)
{
   //drawDot(goal);

   
   //dist G0,T
   float distGT = calcDistBetwixt(goal, target);
   //cout << "distGT " << distGT << endl;
   //angel G0,T
   float angleG0B = calcAngleToTarget(neuron[i].getPt(), goal);
   float angleGT = calcAngleToTarget(target, goal);
   drawArrow(goal, angleGT);
   // cout << "angleGT " << angleGT << endl;
   //G1 = dist + T.size + b.size

   float Xdist = -distGT*sin(angleGT * TORADIAN)*1.5;
   float Ydist =  distGT*cos(angleGT * TORADIAN)*1.5;
   // cout << "Xdist " << Xdist << endl;
   // cout << "Ydist " << Ydist << endl;
   G1.setX(goal.getX() + Xdist);
   G1.setY(goal.getY() + Ydist);
   //dist B,G1
   float distBG1 = calcDistBetwixt(neuron[i].getPt(), G1);
   //cout << "distBG1 " << distBG1 << endl;
   //anlge B,G1
   float angleBG1 = calcAngleToTarget(G1, neuron[i].getPt());
   //cout << "angleBG1 " << angleBG1 << endl;
   // if (distBG1 < 1)
   // neuron[i].setSpeed(0);
   // else
   //   neuron[i].setSpeed(2);
   neuron[i].setAngle(angleBG1);

   if (distBG1 == 0)
      neuron[i].setMutex(!neuron[i].getMutex());
   // neuron[i].setAngle(calcAngleToTarget(goal, neuron[i].getPt()));
   //if((angle G0,T == angle G0,B) && (dist G0,T < dist G0,B) && (dist(dist G0,T)): goTogoal
}

/******************************************
 * convToGoal and obstAvoid
 ******************************************/
void Ganglion::targetToGoal(int i)
{
   
   // if angle bot to target == bot angle
   float angleBG0 = calcAngleToTarget(goal, neuron[i].getPt());
   int distTG0 = calcDistBetwixt(target, goal);
   int distBT = calcDistBetwixt(target, neuron[i].getPt());
   if (distTG0 > 10)
   {
      convToGoal(goal);
      if (distBT < TARGETSIZE - 1)
         targetAdvance(i);
   }
   neuron[i].setAngle(angleBG0);
}

/******************************************
 * convToGoal and obstAvoid
 ******************************************/
void Ganglion::pushObjToGoal()
{
   drawDot(goal);
   drawCircle(target);
   drawKryptonite(G1);
  
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      cout << "bot " << i << " "  << neuron[i].getMutex() << endl;
      float angleBG0 = calcAngleToTarget(goal, neuron[i].getPt());
      float distBG1 = calcDistBetwixt(G1, neuron[i].getPt());
      float distBG0 = calcDistBetwixt(goal, neuron[i].getPt());
      if (neuron[i].getMutex() == 1)
      {
         cout << "bot to temp goal" << endl;
         botToG1(i);
      }
      if (neuron[i].getMutex() == 0)
      {
         cout << "bot to goal" << endl;
         neuron[i].setAngle(angleBG0);
         targetToGoal(i);
      }
      if (distBG0 < TARGETSIZE)
      {
         neuron[i].setSpeed(0);
         neuron[i].setMutex(!neuron[i].getMutex());
      }
      else
         neuron[i].setSpeed(2);
   }
}


/******************************************
 * simply draw a line from bot to closest neighbor
 ******************************************/
void Ganglion::lineBetwixtClosest(int i)
{
   for (int j = 0; j < NUM_NEURONS; j++)
   {
      if (j != i)
      {
         int jClosest = calcClosest(i);
         float angle = calcAngleToTarget(neuron[jClosest].getPt(),
                                         neuron[i].getPt());
         drawLine(neuron[i].getPt(), neuron[jClosest].getPt());
         drawArrow(neuron[i].getPt(), angle);
      }
   }
}

/******************************************
 * Draws line from one bot to all other neighbors. 
 ******************************************/
void Ganglion::lineBetwixtAll()
{
   //for all bots
   for (int i = 0; i < NUM_NEURONS; i++)
   {
      // check all other bots        
      for (int j = 0; j < NUM_NEURONS; j++)
      {
         if (j != i)
         {
            drawLine(neuron[i].getPt(), neuron[j].getPt());
            float angle = calcAngleToTarget(neuron[j].getPt(),
                                            neuron[i].getPt());
            drawArrow(neuron[i].getPt(), angle);
            int diff = angle - neuron[i].getAngle();
         }
      }
   }
}


/******************************************
 * Draws a line from each bot to its closest neightbor
 ******************************************/
int Ganglion::calcClosest(int i)
{
   int jClosest;
   int distBetwixt;
   int minDist;
   //for all bots

   //reintitialize all the minimum distance
   minDist = origin.getXMax() * 3;
        
   for (int j = 0; j < NUM_NEURONS; j++)
   {

      // find dist betwixt
      distBetwixt = calcDistBetwixt(neuron[i].getPt(),
                                    neuron[j].getPt());
         
      if ((distBetwixt < minDist) and (i!=j))
      {
         jClosest = j;
         minDist = distBetwixt;
      }
   }
   
   return jClosest;
}

/******************************************
 * calculate the distance between two points
 ******************************************/
int Ganglion::calcDistBetwixt(Point pt0, Point pt1)
{
   int distBetwixt;
   int xDist = abs(pt0.getX() - pt1.getX());
   int yDist = abs(pt0.getY() - pt1.getY());
   return distBetwixt = sqrt(xDist * xDist + yDist * yDist);
}

/******************************************
 * calculate the desired angel to point pointer towards target
 ******************************************/
float Ganglion::calcAngleToTarget(Point target, Point pointer)
{
   int adj = target.getX() - pointer.getX();
   int op  = target.getY() - pointer.getY();   
   float angle = (atan2(op, adj) * 180 / 3.1415) - 90;
   if (angle < 0)
      angle += 360;
   if (angle > 360)
      angle -= 360;
   return angle;
}


/******************************************
 * make is' angle converg to be equal to the
 * angle of its closest neightor 
 ******************************************/
void Ganglion::convItoJ(int i, int jClosest)
{
   if (neuron[i].getAngle() < neuron[jClosest].getAngle())
      neuron[i].addAngle(1);
   if (neuron[i].getAngle() > neuron[jClosest].getAngle())
      neuron[i].addAngle(-1);
}


/******************************************
 * points bot towards its closest neighbor
 ******************************************/
void Ganglion::convItowardTarget(int i, int jClosest)
{
   /// target - current neuron  

   float angle = calcAngleToTarget(neuron[i].getPt(), neuron[jClosest].getPt());

   float diff = neuron[i].getAngle() - angle;
   cout << "pointer" << i  << "angel"  << diff << endl;
   
   // if (diff < 0)
   //    neuron[i].addAngle(1);
   // if (diff > 0)
   //    neuron[i].addAngle(-1);
   
   // if (diff < 0)
   //    neuron[i].addAngle(1);
   // if (diff > 0)
   //    neuron[i].addAngle(-1);
   
   if (diff < 0)
      neuron[i].addAngle(1);
   if (diff > 0)
      neuron[i].addAngle(-1);
   
   // if (neuron[i].getAngle() < angle)
   //    neuron[i].addAngle(1);
   // if (neuron[i].getAngle() > angle)
   //    neuron[i].addAngle(-1);


   // drawNumber(neuron[i].getPt(),angle);
   drawArrow(neuron[i].getPt(),angle);

}

/******************************************
 * points bot to average of its angel
 * and closest neighbors ang le
 ******************************************/
void Ganglion::convIavgBoth(int i, int jClosest)
{
   int avg = i + jClosest / 2;
   if (neuron[i].getAngle() < avg)
      neuron[i].addAngle(1);
   if (neuron[i].getAngle() > avg)
      neuron[i].addAngle(-1);
}

/*********************************************
 * CALLBACK
 * The main interaction loop of the engine.
 * This gets called from OpenGL.  It give us our
 * interface pointer (where we get our events from)
 * as well as a void pointer which we know is our
 * ganglion class.
 *********************************************/
void callBack(const Interface *pUI, void *p)
{
   Ganglion *ganglion = (Ganglion *)p;
   ganglion->draw();
   ganglion->interact(pUI, p);
   ganglion->increment();
   ganglion->behavior();
  
   return;
}
/*********************************
 * Just initialize the Ganglion class
 * and call the display engine.
 *********************************/
int main(int argc, char ** argv)
{
   Interface ui(argc, argv, "Swarm"); // initialize OpenGL
   Ganglion ganglion;                 // initialize the simultion state
   ui.run(callBack, &ganglion);           // set everything into action

   return 0;
}
