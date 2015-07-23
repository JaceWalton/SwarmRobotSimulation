/***********************************************************************
 * Source File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Jace Walton
 *    Nathaniel Mason
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 *
 *    added Draw functions for Superman, Missile, Laser, Asteroids, Kryptonite
 *    and a Luthor drone. 
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock
#include "point.h"


#define LINUX
//#define MAC_XCODE
//#define WIN_VISUAL_STUDIO

#ifdef MAC_XCODE
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // MAC_XCODE

#ifdef LINUX
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#endif // LINUX

#ifdef WIN_VISUAL_STUDIO
#include <stdio.h>
#include <stdlib.h>
#include "C:\Program Files\Microsoft Visual Studio 12.0\VC\include\glut.h"         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#endif // WIN_VISUAL_STUDIO


#include "point.h"
#include "uiDraw.h"

using namespace std;

#define deg2rad(value) ((M_PI / 180) * (value))
#define SHIP_SIZE 10

/*********************************************
 * NUMBER OUTLINES
 * We are drawing the text for score and things
 * like that by hand to make it look "old school."
 * These are how we render each individual charactger.
 * Note how -1 indicates "done".  These are paired
 * coordinates where the even are the x and the odd
 * are the y and every 2 pairs represents a point
 ********************************************/
const char NUMBER_OUTLINES[10][20] =
{
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   0,10,  0, 0,  -1,-1, -1,-1},//0
  {7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//1
  {0, 0,  7, 0,   7, 0,  7, 5,   7, 5,  0, 5,   0, 5,  0,10,   0,10,  7,10},//2
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   4, 5,  7, 5,  -1,-1, -1,-1},//3
  {0, 0,  0, 5,   0, 5,  7, 5,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1},//4
  {7, 0,  0, 0,   0, 0,  0, 5,   0, 5,  7, 5,   7, 5,  7,10,   7,10,  0,10},//5
  {7, 0,  0, 0,   0, 0,  0,10,   0,10,  7,10,   7,10,  7, 5,   7, 5,  0, 5},//6
  {0, 0,  7, 0,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//7
  {0, 0,  7, 0,   0, 5,  7, 5,   0,10,  7,10,   0, 0,  0,10,   7, 0,  7,10},//8
  {0, 0,  7, 0,   7, 0,  7,10,   0, 0,  0, 5,   0, 5,  7, 5,  -1,-1, -1,-1} //9
};

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawDigit(const Point & topLeft, char digit)
{
   
   // we better be only drawing digits
   assert(isdigit(digit));
   if (!isdigit(digit))
      return;

   // compute the row as specified by the digit
   int r = digit - '0';
   assert(r >= 0 && r <= 9);

   // go through each segment.
   for (int c = 0; c < 20 && NUMBER_OUTLINES[r][c] != -1; c += 4)
   {
      assert(NUMBER_OUTLINES[r][c    ] != -1 &&
             NUMBER_OUTLINES[r][c + 1] != -1 &&
             NUMBER_OUTLINES[r][c + 2] != -1 &&
             NUMBER_OUTLINES[r][c + 3] != -1);

      //Draw a line based off of the num structure for each number
      Point start;
      start.setX(topLeft.getX() + NUMBER_OUTLINES[r][c]);
      start.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 1]);
      Point end;
      end.setX(topLeft.getX() + NUMBER_OUTLINES[r][c + 2]);
      end.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 3]);
         
      drawLine(start, end);
   }
}

/*************************************************************************
 * DRAW NUMBER
 * Display an positive integer on the screen using the 7-segment method
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawNumber(const Point & topLeft, unsigned int number)
{
  
   // render the number as text
   ostringstream sout;
   sout << number;
   string text = sout.str();

   // walk through the text one digit at a time
   Point point = topLeft;

   // this makes digat draw form center
   point.addX(-4);
   point.addY(5);

   for (const char *p = text.c_str(); *p; p++)
   {
      assert(isdigit(*p));
      drawDigit(point, *p);
      point.addX(11);
   }
}

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Point & topLeft, const char * text)
{
   void *pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18

   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());

   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}

/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *    INPUT  point    The point to be moved
 *           center   The center point we will rotate around
 *           rotation Rotation in degrees
 *    OUTPUT point    The new position
 *************************************************************************/
void rotate(Point & point, const Point & origin, int rotation)
{
   // because sine and cosine are expensive, we want to call them only once
   double cosA = cos(deg2rad(rotation));
   double sinA = sin(deg2rad(rotation));

   // remember our original point
   Point tmp(false /*check*/);
   tmp.setX(point.getX() - origin.getX());
   tmp.setY(point.getY() - origin.getY());

   // find the new values
   point.setX(static_cast<int> (tmp.getX() * cosA -
                                tmp.getY() * sinA) +
              origin.getX());
   point.setY(static_cast<int> (tmp.getX() * sinA +
                                tmp.getY() * cosA) +
              origin.getY());
}

/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRect(const Point & center, char width, char height, int rotation)
{
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right 
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right

   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));

   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));

   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));

   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));

   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glEnd();
}

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *          points   How many points will we draw it.  Larger the number,
 *                   the more line segments we will use
 *          rotation True circles are rotation independent.  However, if you
 *                   are drawing a 3-sided circle (triangle), this matters!
 *************************************************************************/
void drawCircle(const Point & center, char radius, int points, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);

   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (double i = 0; i < 2 * M_PI; i += (2 * M_PI) / points)
   {
      Point temp(false /*check*/);
      temp.setX(center.getX() + static_cast<int>(radius * cos(i)));
      temp.setY(center.getY() + static_cast<int>(radius * sin(i)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
	  glColor3f(1 /*%red*/, 1 /*%green*/, 1 /*%blue*/);
   }
   
   // complete drawing
   glEnd();

}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *   INPUT  begin     The position of the beginning of the line
 *          end       The position of the end of the line
 *************************************************************************/
void drawLine(const Point & begin, const Point & end)
{
   // Get ready...
   glBegin(GL_LINES);

   // Note how we scale the lines by a factor of two
   glVertex2f(begin.getX(), begin.getY());
   glVertex2f(  end.getX(),   end.getY());

   // complete drawing.
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDot(const Point & point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);

   // Go...
   glColor3f(1.0, 0.0, 0.0);    //green
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);
   glColor3f(1.0, 1.0, 1.0);    //reset to white 

   // Done!  OK, that was a bit too dramatic
   glEnd();
}

/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShip(const Point & center, int rotation)
{
   // Get ready, get set...
   Point bow(      center); // front
   Point stern(    center); // back
   Point starboard(center); // right
   Point port(     center); // left

   bow.addX(-6);

   stern.addX(2);

   starboard.addY(3);
   starboard.addX(3);

   port.addY(-3);
   port.addX( 3);
   
   //Rotate all points the given degrees
   rotate(bow,       center, rotation);
   rotate(stern,     center, rotation);
   rotate(starboard, center, rotation);
   rotate(port,      center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glColor3f(0.8515625 /*%red*/, 0.64453125 /*%green*/, 0.125 /*%blue*/);  
   glVertex2f(      bow.getX(),       bow.getY());
   glVertex2f(starboard.getX(), starboard.getY());
   glVertex2f(    stern.getX(),     stern.getY());
   glVertex2f(     port.getX(),      port.getY());
   glVertex2f(      bow.getX(),       bow.getY());
   glColor3f (1.0, 1.0, 1.0);  // reset to white

   // Done!  OK, that was a bit too dramatic
   glEnd();   
}

/************************************************************************
* DRAW Rifle
* Draw a rifle on the screen
*  INPUT point   The position of the rifle
*        angle   Which direction it is ponted
*************************************************************************/
void drawRifle(const Point & center, int rotation)
{
	// Get ready, get set...
	Point one(center); // front
	Point two(center); // back
        Point three(center); // right
	Point four(center); // left
	Point five(center);
	Point six(center);

	one.addX(-100);
	one.addY(10);

	two.addX(0);
	two.addY(10);

	three.addX(0);
	three.addY(-10);

	four.addX(-30);
	four.addY(0);

	five.addX(-40);
	five.addY(-10);
	
	six.addX(-45);
	six.addY(0);

	//Rotate all points the given degrees
	rotate(one, center, rotation);
	rotate(two, center, rotation);
	rotate(three, center, rotation);
	rotate(four, center, rotation);
	rotate(five, center, rotation);
	rotate(six, center, rotation);

	//Finally draw the rectangle
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 1.0, 0.0);  //green
	glVertex2f(one.getX(), one.getY());
	glVertex2f(two.getX(), two.getY());
	glVertex2f(three.getX(), three.getY());
	glVertex2f(four.getX(), four.getY());
	glVertex2f(five.getX(), five.getY());
	glVertex2f(six.getX(), six.getY());
	glVertex2f(one.getX(), one.getY());
	glColor3f(1.0, 1.0, 1.0);  // reset to white

	// Done!  OK, that was a bit too dramatic
	glEnd();
}

/************************************************************************
* DRAW Superman
* Draw Superman on the screen
*  INPUT positon
**********************************************************************/
void drawSuperman(const Point & center, int rotation)
{
       // Get ready, get set...
   Point a(false /*check*/);
   Point b(false /*check*/);
   Point c(false /*check*/);
   Point d(false /*check*/);
   Point e(false /*check*/);
   Point f(false /*check*/);
   Point g(false /*check*/);
   Point h(false /*check*/);
   Point i(false /*check*/);
   Point j(false /*check*/);
   Point k(false /*check*/);
   Point l(false /*check*/);
   Point m(false /*check*/);
   Point n(false /*check*/);
   Point o(false /*check*/);
   Point p(false /*check*/);
   Point q(false /*check*/);
   Point r(false /*check*/);
   Point s(false /*check*/);
   Point t(false /*check*/);
   Point u(false /*check*/);
   Point v(false /*check*/);
   Point w(false /*check*/);
   Point x(false /*check*/);
   Point y(false /*check*/);
   Point z(false /*check*/);
   Point aa(false /*check*/);
   Point bb(false /*check*/);
   Point cc(false /*check*/);
   Point dd(false /*check*/);
 
   a.addX(center.getX() - 5);
   a.addY(center.getY() - 50);
 
   b.addX(center.getX() - 5);
   b.addY(center.getY() - 25);
 
   c.addX(center.getX() - 20);
   c.addY(center.getY() - 25);
 
   d.addX(center.getX() - 10);
   d.addY(center.getY() + 25);
 
   e.addX(center.getX() - 15);
   e.addY(center.getY() + 30);
 
   f.addX(center.getX() - 5);
   f.addY(center.getY() + 50);
 
   g.addX(center.getX()     );
   g.addY(center.getY() + 20);
 
   h.addX(center.getX()     ); 
   h.addY(center.getY() + 30);
 
   i.addX(center.getX() -  5);
   i.addY(center.getY() + 35);
 
   j.addX(center.getX()     );
   j.addY(center.getY() + 40);
 
   k.addX(center.getX() +  5); 
   k.addY(center.getY() + 35);
 
   l.addX(center.getX() + 15);
   l.addY(center.getY() + 30);
 
   m.addX(center.getX() +  5);
   m.addY(center.getY() + 50);

   n.addX(center.getX() + 10);
   n.addY(center.getY() + 25);
 
   o.addX(center.getX() + 20);
   o.addY(center.getY() - 25);
 
   p.addX(center.getX() +  5);
   p.addY(center.getY() - 25);
 
   q.addX(center.getX() +  5);
   q.addY(center.getY() - 50);
 
   r.addX(center.getX() +  0);
   r.addY(center.getY() - 10);
 
   s.addX(center.getX() + 10);
   s.addY(center.getY() +  5);
 
   t.addX(center.getX() +  5);
   t.addY(center.getY() + 10);
 
   u.addX(center.getX() -  5); 
   u.addY(center.getY() + 10);
 
   v.addX(center.getX() - 10); 
   v.addY(center.getY() +  5);
 
   w.addX(center.getX()     );
   w.addY(center.getY()-   5);
 
   x.addX(center.getX() -  4); 
   x.addY(center.getY()     );
 
   y.addX(center.getX() +  5);
   y.addY(center.getY() +  2);
 
   z.addX(center.getX() -  7);
   z.addY(center.getY() +  4);

   aa.addX(center.getX() - 4);
   aa.addY(center.getY() + 7);
 
   bb.addX(center.getX() + 4);
   bb.addY(center.getY() + 7);

   cc.addX(center.getX() + 7);
   cc.addY(center.getY() + 4);
   
   dd.addX(center.getX()    );
   dd.addY(center.getX() - 25);
 
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
   rotate(m, center, rotation);
   rotate(n, center, rotation);
   rotate(o, center, rotation);
   rotate(p, center, rotation);
   rotate(q, center, rotation);
   rotate(r, center, rotation);
   rotate(s, center, rotation);
   rotate(t, center, rotation);
   rotate(u, center, rotation);
   rotate(v, center, rotation);
   rotate(w, center, rotation);
   rotate(x, center, rotation);
   rotate(y, center, rotation);
   rotate(z, center, rotation);
   rotate(aa, center, rotation);
   rotate(bb, center, rotation);
   rotate(cc, center, rotation);
   rotate(dd, center, rotation);
 
   //Finally draw SuperMan!!!

   //    draw his body outling
   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glBegin(GL_LINES);
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(e.getX(), e.getY());
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(d.getX(), d.getY());
   glVertex2f(g.getX(), g.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(h.getX(), h.getY());
   glColor3f(1.0, 1.0, 1.0);        //white
   glVertex2f(i.getX(), i.getY());
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glVertex2f(h.getX(), h.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(g.getX(), g.getY());
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(n.getX(), n.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue     
   glVertex2f(l.getX(), l.getY());
   glVertex2f(m.getX(), m.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(n.getX(), n.getY());
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(o.getX(), o.getY());
   glVertex2f(p.getX(), p.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue 
   glVertex2f(q.getX(), q.getY());
   glVertex2f(p.getX(), p.getY());
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(o.getX(), o.getY());
   glVertex2f(n.getX(), n.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(d.getX(), d.getY());
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(e.getX(), e.getY());
   glVertex2f(d.getX(), d.getY());
   glEnd();
   
   glBegin(GL_LINES);              // this syntax enalbes me to draw differant
   glEnd();                        // entities in the same drawing. 

  
   //draw the shield on his cape
   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 0.0 , 1.0);       //blue
   glVertex2f(r.getX(), r.getY());
   glVertex2f(s.getX(), s.getY());
   glVertex2f(t.getX(), t.getY());
   glVertex2f(u.getX(), u.getY());
   glVertex2f(v.getX(), v.getY());
   glVertex2f(r.getX(), r.getY());
   glEnd();

   // draw the 'S' in the shield
   glBegin(GL_LINE_STRIP);           
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(w.getX(), w.getY());
   glVertex2f(x.getX(), x.getY());
   glVertex2f(w.getX(), w.getY());
   glVertex2f(y.getX(), y.getY());
   glVertex2f(z.getX(), z.getY());
   glVertex2f(aa.getX(), aa.getY());
   glVertex2f(bb.getX(), bb.getY());
   glVertex2f(cc.getX(), cc.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white
 
   // Done!  OK, that was a bit too dramatic
   glEnd();
}

/************************************************************************
* DRAW Lasers
* Draw lasers on the screen
*  INPUT 
**********************************************************************/
void drawLasers(const Point & center, int rotation)
{
   // Get ready...
   Point a(center);
   Point b(center);
   Point c(center);
   Point d(center);
  
  
   a.addX(-3);
   a.addY(-15);
 
   b.addX(-3);
   b.addY(15);
 
   c.addX(  3);
   c.addY(-15);
 
   d.addX( 3);
   d.addY(15);
 
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);

   // draw the two lasers  one from each of Supermans eyes
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glEnd();

  
   glBegin(GL_LINE_STRIP);
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   // complete drawing.
   glEnd();
}

/************************************************************************
* DRAW AsteroidLrg
* Draw Large Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawAsteroidLrg(const Point & center, int rotation)
{
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   Point k(false);
   Point l(false);
   Point m(false);
   Point n(false);
   Point o(false);
   Point p(false);
   Point q(false);
   Point r(false);
   Point s(false);
   Point t(false);
   Point u(false);
   Point v(false);
   Point w(false);
   Point x(false);
   Point y(false);
   Point z(false);
 
   a.addX(center.getX() + 20);
   a.addY(center.getY() + 50);
 
   b.addX(center.getX() + 0);
   b.addY(center.getY() + 50);
 
   c.addX(center.getX() - 15);
   c.addY(center.getY() + 40);
 
   d.addX(center.getX() + -10);
   d.addY(center.getY() + 20);
 
   e.addX(center.getX() - 40);
   e.addY(center.getY() + 30);
 
   f.addX(center.getX() - 50);
   f.addY(center.getY() + 20);
 
   g.addX(center.getX() - 50);
   g.addY(center.getY() + 10);
 
   h.addX(center.getX() - 40); 
   h.addY(center.getY() + 5);
 
   i.addX(center.getX() - 30);
   i.addY(center.getY() + 10);
 
   j.addX(center.getX() - 20);
   j.addY(center.getY() + 5);
 
   k.addX(center.getX() - 40); 
   k.addY(center.getY() - 10);
 
   l.addX(center.getX() - 40);
   l.addY(center.getY() - 20);
 
   m.addX(center.getX() - 30);
   m.addY(center.getY() - 25);

   n.addX(center.getX() - 30);
   n.addY(center.getY() - 30);
 
   o.addX(center.getX() - 20);
   o.addY(center.getY() - 40);
 
   p.addX(center.getX() - 15);
   p.addY(center.getY() - 50);
 
   q.addX(center.getX() - 5);
   q.addY(center.getY() - 50);
 
   r.addX(center.getX() + 10);
   r.addY(center.getY() - 40);
 
   s.addX(center.getX() + 20);
   s.addY(center.getY() - 50);
 
   t.addX(center.getX() + 50);
   t.addY(center.getY() - 40);
 
   u.addX(center.getX() + 30); 
   u.addY(center.getY() - 10);
 
   v.addX(center.getX() + 40); 
   v.addY(center.getY() + 10);
 
   w.addX(center.getX() + 30);
   w.addY(center.getY() + 20);
 
   x.addX(center.getX() + 35); 
   x.addY(center.getY() + 30);
 
   y.addX(center.getX() + 20);
   y.addY(center.getY() + 20);
 
   z.addX(center.getX() + 10);
   z.addY(center.getY() + 40);

   
 
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
   rotate(m, center, rotation);
   rotate(n, center, rotation);
   rotate(o, center, rotation);
   rotate(p, center, rotation);
   rotate(q, center, rotation);
   rotate(r, center, rotation);
   rotate(s, center, rotation);
   rotate(t, center, rotation);
   rotate(u, center, rotation);
   rotate(v, center, rotation);
   rotate(w, center, rotation);
   rotate(x, center, rotation);
   rotate(y, center, rotation);
   rotate(z, center, rotation);

     
   //draw the asteroid
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 1.0, 1.0);        // white
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(m.getX(), m.getY());
   glVertex2f(n.getX(), n.getY());
   glVertex2f(o.getX(), o.getY());
   glVertex2f(p.getX(), p.getY());
   glVertex2f(q.getX(), q.getY());
   glVertex2f(r.getX(), r.getY());
   glVertex2f(s.getX(), s.getY());
   glVertex2f(t.getX(), t.getY());
   glVertex2f(u.getX(), u.getY());
   glVertex2f(v.getX(), v.getY());
   glVertex2f(w.getX(), w.getY());
   glVertex2f(x.getX(), x.getY());
   glVertex2f(y.getX(), y.getY());
   glVertex2f(z.getX(), z.getY());
   glVertex2f(a.getX(), a.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}

/************************************************************************
* DRAW AsteroidMed
* Draw Medium Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawAsteroidMed(const Point & center, int rotation)
{
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   Point k(false);
   Point l(false);
   Point m(false);
   Point n(false);
   Point o(false);
   Point p(false);
   Point q(false);
 
   a.addX(center.getX() + 0);
   a.addY(center.getY() + 20);
 
   b.addX(center.getX() - 10);
   b.addY(center.getY() + 35);
 
   c.addX(center.getX() - 30);
   c.addY(center.getY() + 15);
 
   d.addX(center.getX() - 30);
   d.addY(center.getY() + 5);
 
   e.addX(center.getX() - 20);
   e.addY(center.getY() - 5);
 
   f.addX(center.getX() - 30);
   f.addY(center.getY() - 25);
 
   g.addX(center.getX() - 15);
   g.addY(center.getY() - 25);
 
   h.addX(center.getX() - 10); 
   h.addY(center.getY() - 35);
 
   i.addX(center.getX() + 25);
   i.addY(center.getY() - 15);
 
   j.addX(center.getX() + 30);
   j.addY(center.getY() - 5);
 
   k.addX(center.getX() + 20); 
   k.addY(center.getY() + 5);
 
   l.addX(center.getX() + 20);
   l.addY(center.getY() + 15);
 
   m.addX(center.getX() + 30);
   m.addY(center.getY() + 15);

   n.addX(center.getX() + 30);
   n.addY(center.getY() + 25);
 
   o.addX(center.getX() + 20);
   o.addY(center.getY() + 35);
 
   p.addX(center.getX() + 15);
   p.addY(center.getY() + 35);
 
   q.addX(center.getX() + 10);
   q.addY(center.getY() + 25);
 
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
   rotate(m, center, rotation);
   rotate(n, center, rotation);
   rotate(o, center, rotation);
   rotate(p, center, rotation);
   rotate(q, center, rotation);

   //draw the asteroid
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 1.0, 1.0);        // white
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(m.getX(), m.getY());
   glVertex2f(n.getX(), n.getY());
   glVertex2f(o.getX(), o.getY());
   glVertex2f(p.getX(), p.getY());
   glVertex2f(q.getX(), q.getY());
   glVertex2f(a.getX(), a.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}

/************************************************************************
* DRAW AsteroidLrg
* Draw Large Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawAsteroidSml(const Point & center, int rotation)
{
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   Point k(false);
   Point l(false);
   Point m(false);
   Point n(false);
   Point o(false);
   Point p(false);
   Point q(false);
   Point r(false);
   Point s(false);
   Point t(false);
   Point u(false);
   Point v(false);
  
 
   a.addX(center.getX() + 5);
   a.addY(center.getY() + 15);
 
   b.addX(center.getX() + 0);
   b.addY(center.getY() + 10);
 
   c.addX(center.getX() - 3); 
   c.addY(center.getY() + 5);
 
   d.addX(center.getX() - 3);
   d.addY(center.getY() + 15);
 
   e.addX(center.getX() - 5);
   e.addY(center.getY() + 15);
 
   f.addX(center.getX() - 10);
   f.addY(center.getY() + 5);

   g.addX(center.getX() - 15);
   g.addY(center.getY() + 3);  
 
   h.addX(center.getX() - 15); 
   h.addY(center.getY() - 3);  
 
   i.addX(center.getX() - 10);  
   i.addY(center.getY() - 3);   
 
   j.addX(center.getX() - 15);
   j.addY(center.getY() - 10);
 
   k.addX(center.getX() - 5); 
   k.addY(center.getY() - 5);
 
   l.addX(center.getX() - 3);  //???
   l.addY(center.getY() - 5);
 
   m.addX(center.getX() + 0);
   m.addY(center.getY() - 15);

   n.addX(center.getX() + 10);
   n.addY(center.getY() - 10);

   o.addX(center.getX() + 15);
   o.addY(center.getY() - 10);
 
   p.addX(center.getX() + 5);
   p.addY(center.getY() - 5);
 
   q.addX(center.getX() + 10);
   q.addY(center.getY() - 0);

   r.addX(center.getX() + 10);
   r.addY(center.getY() + 5);
 
   s.addX(center.getX() + 15);
   s.addY(center.getY() + 10);
 
   t.addX(center.getX() + 5);
   t.addY(center.getY() + 5);
 
   u.addX(center.getX() + 5); 
   u.addY(center.getY() + 5);
 
   v.addX(center.getX() + 10); 
   v.addY(center.getY() + 15);
 
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
   rotate(m, center, rotation);
   rotate(n, center, rotation);
   rotate(o, center, rotation);
   rotate(p, center, rotation);
   rotate(q, center, rotation);
   rotate(r, center, rotation);
   rotate(s, center, rotation);
   rotate(t, center, rotation);
   rotate(u, center, rotation);
   rotate(v, center, rotation);

   //draw the Asteroid
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 1.0, 1.0);        // white
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(m.getX(), m.getY());
   glVertex2f(n.getX(), n.getY());
   glVertex2f(o.getX(), o.getY());
   glVertex2f(p.getX(), p.getY());
   glVertex2f(q.getX(), q.getY());
   glVertex2f(r.getX(), r.getY());
   glVertex2f(s.getX(), s.getY());
   glVertex2f(t.getX(), t.getY());
   glVertex2f(u.getX(), u.getY());
   glVertex2f(v.getX(), v.getY());
   glVertex2f(a.getX(), a.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}

/************************************************************************
* DRAW AsteroidMed
* Draw Medium Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawMissile(const Point & center, int rotation)
{
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   Point k(false);
   Point l(false);
   Point m(false);
   Point n(false);
   Point o(false);
   Point p(false);
   Point q(false);
   Point r(false);
   Point s(false);
   Point t(false);
   Point u(false);
   Point v(false);
   Point w(false);
   Point x(false);
   Point y(false);
 
   a.addX(center.getX() + 0);
   a.addY(center.getY() + 25);
 
   b.addX(center.getX() - 3);
   b.addY(center.getY() + 15);
 
   c.addX(center.getX() + 3);
   c.addY(center.getY() + 15);
 
   d.addX(center.getX() + 3);
   d.addY(center.getY() - 20);
 
   e.addX(center.getX() - 3);
   e.addY(center.getY() - 20);
 
   f.addX(center.getX() + 3);
   f.addY(center.getY() + 10);
 
   g.addX(center.getX() + 7);
   g.addY(center.getY() + 7);
 
   h.addX(center.getX() + 7); 
   h.addY(center.getY() + 0);
 
   i.addX(center.getX() + 3);
   i.addY(center.getY() + 3);
 
   j.addX(center.getX() - 3);
   j.addY(center.getY() + 10);
 
   k.addX(center.getX() - 7);
   k.addY(center.getY() + 7);
 
   l.addX(center.getX() - 7); 
   l.addY(center.getY() + 0);
 
   m.addX(center.getX() - 3);
   m.addY(center.getY() + 3);

   n.addX(center.getX() + 3);
   n.addY(center.getY() - 5);
 
   o.addX(center.getX() + 10);
   o.addY(center.getY() - 15);
 
   p.addX(center.getX() + 10);
   p.addY(center.getY() - 25);  
 
   q.addX(center.getX() + 3);
   q.addY(center.getY() - 15);

   r.addX(center.getX() - 3);
   r.addY(center.getY() - 5);
 
   s.addX(center.getX() - 10);
   s.addY(center.getY() - 15);
 
   t.addX(center.getX() - 10);
   t.addY(center.getY() - 25);   
 
   u.addX(center.getX() - 3);
   u.addY(center.getY() - 15);

   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
   rotate(m, center, rotation);
   rotate(n, center, rotation);
   rotate(o, center, rotation);
   rotate(p, center, rotation);
   rotate(q, center, rotation);
   rotate(r, center, rotation);
   rotate(s, center, rotation);
   rotate(t, center, rotation);
   rotate(u, center, rotation);
   rotate(v, center, rotation);
   rotate(w, center, rotation);
   rotate(x, center, rotation);
   rotate(y, center, rotation);

   //draw the missile
   glBegin(GL_LINE_STRIP);
   
   //tip
   glColor3f(1.0, 0.0, 0.0);        // red
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(a.getX(), a.getY());
   glVertex2f(c.getX(), c.getY());
   //body
   glColor3f(0.0, 1.0, 0.0);        // green
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(b.getX(), b.getY());
   glEnd();

   //fins
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        // red
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glEnd();

   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        // red
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(m.getX(), m.getY());
   glEnd();
   
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        // red
   glVertex2f(n.getX(), n.getY());
   glVertex2f(o.getX(), o.getY());
   glVertex2f(p.getX(), p.getY());
   glVertex2f(q.getX(), q.getY());
   glEnd();

   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        // red
   glVertex2f(r.getX(), r.getY());
   glVertex2f(s.getX(), s.getY());
   glVertex2f(t.getX(), t.getY());
   glVertex2f(u.getX(), u.getY());
   glEnd();
   
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}

/************************************************************************
* DRAW Lasers
* Draw lasers on the screen
*  INPUT 
**********************************************************************/
void drawKryptonite(const Point & center, int rotation)
{
   // Get ready...
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
  
   a.addX(center.getX() + 0);
   a.addY(center.getY() + 50);
 
   b.addX(center.getX() - 7);
   b.addY(center.getY() + 10);
 
   c.addX(center.getX() - 20);
   c.addY(center.getY() - 10);
 
   d.addX(center.getX() + 0);
   d.addY(center.getY() - 50);

   e.addX(center.getX() + 20);
   e.addY(center.getY() - 10);
 
   f.addX(center.getX() + 20);
   f.addY(center.getY() + 10);
   
   g.addX(center.getX() + 7);
   g.addY(center.getY() - 10);
 
   h.addX(center.getX() - 20);
   h.addY(center.getY() + 10);
 
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);

   // draw the two lasers  one from each of Supermans eyes
   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 0.5, 0.0);        //green
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(e.getX(), e.getY());
   glColor3f(0.0, 1.0, 0.0);
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(a.getX(), a.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(d.getX(), d.getY());
   glColor3f(1.0, 1.0, 1.0);

   // complete drawing.
   glEnd();
}



/******************************************************************
 *
 ******************************************************************/
void drawDrone(const Point & center, int rotation)
{
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   
  
   a.addX(center.getX() - 10);
   a.addY(center.getY() - 30);
 
   b.addX(center.getX()     );
   b.addY(center.getY() - 10);
 
   c.addX(center.getX() + 10);
   c.addY(center.getY() - 30);
 
   d.addX(center.getX()     );
   d.addY(center.getY() + 30);

   e.addX(center.getX() + 30);
   e.addY(center.getY() - 10);
 
   f.addX(center.getX() - 30);
   f.addY(center.getY() - 10);
   
   g.addX(center.getX() - 30);
   g.addY(center.getY()     );
 
   h.addX(center.getX() + 30);
   h.addY(center.getY()     );

   i.addX(center.getX() + 30);
   i.addY(center.getY() + 30);
 
   j.addX(center.getX() - 30);
   j.addY(center.getY() + 30);
   

   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);

   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);        //red
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(a.getX(), a.getY());
   glEnd();

   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 0.0, 1.0);        //blue
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(e.getX(), e.getY());
   glEnd();

   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 1.0, 0.0);        //green
   glVertex2f(h.getX(), h.getY());
   glVertex2f(i.getX(), i.getY());
   glEnd();

   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 1.0, 0.0);        //green
   glVertex2f(g.getX(), g.getY());
   glVertex2f(j.getX(), j.getY());
   glEnd();
   
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(d.getX(), d.getY());

   // complete drawing.*/
   glEnd();
   
   return;
 
}

/************************************************************************
* DRAW AsteroidLrg
* Draw Large Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawBot(const Point & center, int rotation)
{

   drawCircle(center,  20, 20, rotation);
   
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);
   Point e(false);
   Point f(false);
   Point g(false);
   Point h(false);
   Point i(false);
   Point j(false);
   Point k(false);
   Point l(false);
  
 
   a.addX(center.getX() + 8);
   a.addY(center.getY() + 20);
 
   b.addX(center.getX() + 8);
   b.addY(center.getY() + 40);
 
   c.addX(center.getX() + 16);
   c.addY(center.getY() + 48);
 
   d.addX(center.getX() + 8);
   d.addY(center.getY() + 56);
 
   e.addX(center.getX()     );
   e.addY(center.getY() + 56);
 
   f.addX(center.getX() + 8 );
   f.addY(center.getY() + 48);
 
   g.addX(center.getX()     );
   g.addY(center.getY() + 40);
 
   h.addX(center.getX() - 8 ); 
   h.addY(center.getY() + 48);
 
   i.addX(center.getX() - 8 );
   i.addY(center.getY() + 56);
 
   j.addX(center.getX() - 16);
   j.addY(center.getY() + 48);
 
   k.addX(center.getX() - 8 ); 
   k.addY(center.getY() + 40);
 
   l.addX(center.getX() - 8 );
   l.addY(center.getY() + 20);
 
  
   
 
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
   rotate(e, center, rotation);
   rotate(f, center, rotation);
   rotate(g, center, rotation);
   rotate(h, center, rotation);
   rotate(i, center, rotation);
   rotate(j, center, rotation);
   rotate(k, center, rotation);
   rotate(l, center, rotation);
  
     
   //draw the asteroid
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 1.0, 1.0);        // white
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(f.getX(), f.getY());
   glVertex2f(g.getX(), g.getY());
   glVertex2f(h.getX(), h.getY());
   glVertex2f(e.getX(), e.getY());
   glVertex2f(i.getX(), i.getY());
   glVertex2f(j.getX(), j.getY());
   glVertex2f(k.getX(), k.getY());
   glVertex2f(l.getX(), l.getY());
   glVertex2f(a.getX(), a.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}


/************************************************************************
* DRAW AsteroidLrg
* Draw Large Asteroid on the screen
*  INPUT 
**********************************************************************/
void drawArrow(const Point & center, int rotation)
{

   // drawCircle(center,  20, 20, rotation);
   
   Point a(false);
   Point b(false);
   Point c(false);
   Point d(false);

 
   a.addX(center.getX() + 12);
   a.addY(center.getY() + 20);
 
   b.addX(center.getX() + 0);
   b.addY(center.getY() + 44);
 
   c.addX(center.getX() - 12);
   c.addY(center.getY() + 20);
 
   d.addX(center.getX() + 0);
   d.addY(center.getY() + 28);


   
   //Rotate all points the given degrees
   rotate(a, center, rotation);
   rotate(b, center, rotation);
   rotate(c, center, rotation);
   rotate(d, center, rotation);
  
     
   //draw the asteroid
   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 1.0, 0.0);        // white
   glVertex2f(a.getX(), a.getY());
   glVertex2f(b.getX(), b.getY());
   glVertex2f(c.getX(), c.getY());
   glVertex2f(d.getX(), d.getY());
   glVertex2f(a.getX(), a.getY());
   glColor3f(1.0, 1.0, 1.0);        // reset to white

   //compleat the asteroiod
   glEnd();
}


/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min <= max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the double
 ****************************************************************/
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   
   assert(min <= num && num <= max);

   return num;
}

