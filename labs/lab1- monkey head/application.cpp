#include "application.h"

#include <iostream>
#include <cassert>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <fstream>
#include <istream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;



application::application()
{
}

application::~application()
{
}

// triggered once after the OpenGL context is initialized
void application::init_event()
{

    cout << "CAMERA CONTROLS: \n  LMB: Rotate \n  MMB: Move \n  RMB: Zoom" << endl;
    cout << "KEYBOARD CONTROLS: \n  '=': Toggle wireframe mode" << endl;

    const GLfloat ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    const GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // enable a light
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    // enable depth-testing, colored materials, and lighting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // normalize normals so lighting calculations are correct
    // when using GLUT primitives
    glEnable(GL_NORMALIZE);

    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    set_camera_for_box(vec3(-1,-1,-1),vec3(1,1,1));
}


//need to open file 
//get data from file into vector as doubles
//cycle through vector put as vertex
void open_get_from_file()
{
    vector<string> MONKEYLINES;
    string line;
    ifstream myfile("monkey.raw");
    if(!myfile) //Always test the file open.
     {
	std::cerr<<"Error opening output file"<< std::endl;
	return;
     }
     //This gets the lines from my file as a string
    while( getline(myfile, line ) )
    {
	 //copies the string into a cstring
	char* CSTRING = new char [line.length()+1];
	 strcpy (CSTRING, line.c_str());
        
	//this part of the code is using ctoken to parse my cstring
         float one, two, three, four, five, six, seven, eight, nine;
	    one = two = three = four = five = six = seven = eight = nine = 0;
	    char * Number;
	    Number = strtok (CSTRING," "); 
            one = atof(Number);
	    while (Number != NULL)
	    {
	       Number = strtok (NULL, " ");

	       if(two == 0)
 	       {
		   two = atof(Number);
	       }
	       else if(three == 0)
 	       {
		   three= atof(Number);
	       }
	       else if(four == 0)
 	       {
		   four = atof(Number);
	       }
	       else if(five == 0)
 	       {
		   five = atof(Number);
	       }
	       else if(six == 0)
 	       {
		   six = atof(Number);
	       }
	       else if(seven == 0)
 	       {
		  seven = atof(Number);
	       }
	       else if(eight == 0)
 	       {
		   eight = atof(Number);
	       }
	       else if(nine == 0)
 	       {
		   nine = atof(Number);
	       }
	    }

            //drawing the triangle
            glPushMatrix();
	    glBegin(GL_TRIANGLES);
	    glColor3f(1,1,1); glVertex3f(one, two, three);
	    glColor3f(1,1,1); glVertex3f(four, five, six);
	    glColor3f(1,1,1); glVertex3f(seven, eight, nine);
	    glEnd();
	    glPopMatrix();  
	 
    }
   
}





// triggered each time the application needs to redraw
void application::draw_event()
{
    // apply our camera transformation
    apply_gl_transform();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //  this will enable wire-frame mode
    // draw first triangle
    open_get_from_file();



}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(int x, int y)
{
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
}