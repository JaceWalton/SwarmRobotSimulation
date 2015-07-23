/****************************************
 * UI TEST
 * Just a simple program to test the user
 * interface consisting of uiDraw and uiInteract
 ***************************************/


#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include <cmath>
using namespace std;


/************************************
 * Test structure to capture the ball
 * that I will move around the screen
 ***********************************/
class Ball
{
public:
   Ball() : rotation(0), superman(), las(), misl(), drone(),
            astLrg(), astMed(), astSml(), kryp(), acc(20), v(1)  { }

   // this is just for test purposes.  Don't make member variables public!
   void wrap(Point & point);  //wraping all points
   void homing(Point & point1, Point & point2); 
   int getAngle(); /// the angel of the missile
   Point drone;
   Point misl;
   Point kryp;
   Point astSml;
   Point astMed;
   Point astLrg;
   Point las;
   Point superman;          // location of the polygon on the screen
   int rotation;      // the angle or orientation of the polygon
   int acc;
   int v;
};

/************************************
 * a Test function to point the homing 
 * missile at superman wherever he is
 ***********************************/
int Ball::getAngle()
{   
   int a = superman.getX() - misl.getX();
   int o = superman.getY() - misl.getY();
   int angle = atan2(o, a) * 180 / 3.1415;

   return angle - 90;
}

/************************************
 * a Test function to see how to wrap 
 * any and all objects
 ***********************************/
void Ball::wrap(Point & point)
{
   if (point.getY() >= 400)
      point.setY(point.getY() - 800);

   if (point.getY() <= -400)
      point.setY(point.getY() + 800);
   
   if (point.getX() >= 400)
      point.setX(point.getX() - 800);
   
   if (point.getX() <= -400)
      point.setX(point.getX() + 800);
}

/************************************
 * controles a homing missile
 * 
 ***********************************/
void Ball::homing(Point & point1, Point & point2)
{
   int dist = acc/2;
   
   bool a = point1.getX() - point2.getX() <=  dist;
   bool b = point1.getX() - point2.getX() >= -dist;
   bool c = point1.getY() - point2.getY() <=  dist;
   bool d = point1.getY() - point2.getY() >= -dist;

   if (!a||!b)
   if (point1.getX() > point2.getX())
      point2.setX(point2.getX() + dist);
   else
      point2.setX(point2.getX() - dist);
   if (!c||!d)
   if (point1.getY() > point2.getY())
      point2.setY(point2.getY() + dist);
   else
      point2.setY(point2.getY() - dist);
}

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface *pUI, void * p)
{
   Ball * pBall = (Ball *)p;  // cast the void pointer into a known type
   
   
   // move Superman
   if (pUI->isRight())
      pBall->superman.addX(pBall->acc);
      // pBall->rotation;
   if (pUI->isLeft())
      pBall->superman.addX(-pBall->acc);
      // -pBall->rotation;
   if (pUI->isUp())
      pBall->superman.addY(pBall->acc);
   if (pUI->isDown())
      pBall->superman.addY(-pBall->acc);
   
   // accelorates Superman
   if (pUI->isSpace())
   {
      pBall->v++;
      pBall->las.setX(pBall->superman.getX());
      pBall->las.setY(pBall->superman.getY() + 50);
   }

   float GLmatrix[16];
   
   //accelotate constantly
   //pBall->acc++;
   
   // rotate constantly
   pBall->rotation++;

  

   // Move objects
   pBall->las.setX(pBall->las.getX());
   pBall->las.setY(pBall->las.getY() + 20);
   
   pBall->kryp.setX(pBall->kryp.getX() + 5); 
   pBall->kryp.setY(pBall->kryp.getY() + 5);

   pBall->astLrg.setX(pBall->astLrg.getX() + 10);
   pBall->astLrg.setY(pBall->astLrg.getY());

   pBall->astMed.setX(pBall->astMed.getX() - 10);
   pBall->astMed.setY(pBall->astMed.getY());

   pBall->astSml.setX(pBall->astSml.getX());
   pBall->astSml.setY(pBall->astSml.getY() - 5);

   pBall->drone.setX(pBall->drone.getX() - 5);
   pBall->drone.setY(pBall->drone.getY() - 5);


   int angle = pBall->getAngle();    
  
   
   // draw
   drawSuperman       (pBall->superman, 0);
   drawLasers         (pBall->las, 0);
   drawKryptonite     (pBall->kryp, 0);//pBall->rotation);
   drawAsteroidLrg    (pBall->astLrg, pBall->rotation);
   drawAsteroidMed    (pBall->astMed, pBall->rotation);
   drawAsteroidSml    (pBall->astSml, pBall->rotation);
   drawMissile        (pBall->misl, angle );//pBall->rotation);
   drawDrone          (pBall->drone, 0);
  
   // wraps any drawn object by passing it to another function
   pBall->wrap(pBall->superman);
   pBall->wrap(pBall->las);
   pBall->wrap(pBall->astLrg);
   pBall->wrap(pBall->astMed);
   pBall->wrap(pBall->astSml);
   pBall->wrap(pBall->kryp);
   pBall->wrap(pBall->misl);
   pBall->wrap(pBall->drone);

   // the missile follow superman around
   pBall->homing(pBall->superman, pBall->misl);
}



/*********************************
 * Main is pretty sparse.  Just initialize
 * my ball type and call the display engine.
 * That is all!
 *********************************/
int main(int argc, char ** argv)
{
   Interface ui(argc, argv, "Superman in Space");    // initialize OpenGL
   Ball ball;                           // initialize the game state
   ui.run(callBack, &ball);             // set everything into action

   return 0;
}
