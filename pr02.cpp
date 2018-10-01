#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
#define _USE_MATH_DEFINES
#define IMG_SIZE 800

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================


class img{
public:
    int width;
    int height;
    int depth;
    unsigned char *pixels;

    img(int width,int height,int depth)
    {
        this->width=width;
        this->height=height;
        this->depth=depth;
        pixels=new unsigned char[width*height*3];
        memset(pixels,0,width*height*3);
    }
    img(void)
    {
        width=height=depth=0;
    }
};

img pro;


int circle(double x0,double y0,double r,int x,int y)
{
    
  return (x+0.5-x0)*(x+0.5-x0)+(y+0.5-y0)*(y+0.5-y0)-r*r;
}

int line(double theta, double p,int x,int y)
{
  x+=0.5;
  y+=0.5;
  return x*cos(theta)+y*sin(theta)-p;
}
double ellipse(int c1,int c2,double a, double b,int x, int y)
{
  x+=0.5;
  y+=0.5;
  return (x-c1)*(x-c1)*1.0/(a*1.0*a)+(y-c2)*(y-c2)*1.0/(b*1.0*b)-1;
}

void setPixels()
{
  pro=img(800,800,255);
  int rRand=rand()%255;
  int gRand=rand()%255;
  int bRand=rand()%255;
  int r=pro.height/4+rand()%(pro.height/4);

  int step=2+rand()%3;
  for(int e=1;e<=step;e++)
  {
    
    for(int j=0;j<pro.height;j++)
    {
      for(int i=0;i<pro.width;i++)
      {
        int k=(pro.width*j+i)*3;
        if(line(M_PI/2,800*1.0/step*e,i,j)<0&&line(M_PI/2,800*1.0/step*(e-1),i,j)>=0)
        {
            pro.pixels[k++]=e*rRand%255;
            pro.pixels[k++]=e*gRand%255;
            pro.pixels[k]=e*bRand%255;
        }       
      }
    }
  }
  for(int e=1;e<=step;e++)
  {
    
    for(int j=0;j<pro.height;j++)
    {
      for(int i=0;i<pro.width;i++)
      {
        int k=(pro.width*j+i)*3;
        if(line(0,800*1.0/step*e,i,j)<0&&line(0,800*1.0/step*(e-1),i,j)>=0)
        {
            pro.pixels[k++]+=e*gRand%255;
            pro.pixels[k++]+=e*bRand%255;
            pro.pixels[k]+=e*rRand%255;
        }       
      }
    }
  }

  int rrRand=rand()%205+50;
  int ggRand=rand()%205+50;
  int bbRand=rand()%205+50;

  double a=800*1.0/step/2.3;
  double b=800*1.0/step/2;

  for(int j=0;j<pro.height;j++)
  {
    for(int i=0;i<pro.width;i++)
    {
      int k=(pro.width*j+i)*3;

      for(int ex=1;ex<=step;ex++)
        for(int ey=1;ey<=step;ey++)
        {
          if(circle(400*1.0/step*(2*ex-1),400*1.0/step*(2*ey-1)-70*1.0/step,800*1.0/step/3*1.0,i,j)<0)
          {
            pro.pixels[k]=(pro.pixels[k]+ex/step*rrRand)%255;
            pro.pixels[k+1]=(pro.pixels[k+1]+ey/step*ggRand)%255;
            pro.pixels[k+2]=(sin(0.05*(k+2))*1.0+1)/2*bbRand;
          }
          int c1=400*1.0/step*(2*ex-1);
          int c2=400*1.0/step*(2*ey-1)-50*1.0/step;
          if(ellipse(c1,c2,a,b,i,j)<0)
          {
            //if(circle(400*1.0/step*(2*ex-1),400*1.0/step*(2*ey-1)-70*1.0/step,800*1.0/step/3,i,j)>=0)
            //{
              if(line(M_PI/2,400*1.0/step*(2*ey-1)-120*1.0/step,i,j)>=0)
              {
                if(line(0,400*1.0/step*(2*ex-1),i,j)>=0)
                {
                  pro.pixels[k]=0xff-50;
                  pro.pixels[k+1]=0xff-50;
                  pro.pixels[k+2]=0xff-50;
                }
                else
                {
                  pro.pixels[k]=50;
                  pro.pixels[k+1]=50;
                  pro.pixels[k+2]=50;
                }
              }
            //}
          }
        }
    }
  }

}


void ppm_store()
{
  ofstream fout;
  fout.open("procedural_result.ppm",ios::binary | ios::out);
  fout<<"P6"<<endl;
  fout<<IMG_SIZE<<" "<<IMG_SIZE<<endl;
  fout<<"255";
  char value=10;
  fout.write(reinterpret_cast<char*>(&value),1);   //end of magic 
  for(int j=pro.height-1;j>=0;j--)
  {
    for(int i=0;i<pro.width;i++)
    {
      int k=(pro.width*j+i)*3;
    
      fout.write(reinterpret_cast<char*>(&pro.pixels[k++]),1);
      fout.write(reinterpret_cast<char*>(&pro.pixels[k++]),1);
      fout.write(reinterpret_cast<char*>(&pro.pixels[k]),1);

    }
  }
    

}


// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
static void windowResize(int w, int h)
{   
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(w/2),0,(h/2),0,1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity() ;
}
static void windowDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(0,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glDrawPixels(pro.width, pro.height, GL_RGB, GL_UNSIGNED_BYTE, pro.pixels);
  glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
  if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    setPixels();               // Exit on mouse click.
    ppm_store();
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
  }
}
static void init(void)
{
  glClearColor(1,1,1,1); // Set background color.
}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables

  srand(time(NULL));
  setPixels();
  ppm_store();

  // OpenGL Commands:
  // Once "glutMainLoop" is executed, the program loops indefinitely to all
  // glut functions.  
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(pro.width, pro.height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Homework Zero");
  init();
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutMainLoop();
  
  return 0; //This line never gets reached. We use it because "main" is type int.
}

