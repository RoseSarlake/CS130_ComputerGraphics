// Name:
// Quarter, Year:
// Lab:
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <vector>
#include <cstdio>
#include <math.h>
#include "vec.h"
#include <iostream>

using namespace std;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

std::vector<GLfloat> ClickX;
std::vector<GLfloat> ClickY;
bool clicked;

int factorial(int i )
{
	if(i <= 1)
	{
		return 1;
	}
	
	return factorial(i - 1 ) * i;
}



void GL_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.0f);
    // just for example, remove if desired
    /*
    glVertex2f(-.5f,-.5f);
    glVertex2f(.5f,-.5f);
    glVertex2f(.5f,.5f);
    glVertex2f(-.5f,.5f);
    */
       
	if(ClickX.size() >= 2)
	{
		std::vector<GLfloat> glClickX;
		std::vector<GLfloat> glClickY;
		float n = ClickX.size()-1;
		bool first_time = true;
		for(float t = 0.00; t <= 1; t = t + .01 )
		{
			float point_i_x = 0;
			float point_i_y = 0;
			//summation
			for(unsigned i = 0; i < ClickX.size(); i++)
			{
				float n_exc= 0; 
				float k_exc = 0;
				float n_k_exc = 0;
				//use for loop to do this --> n! / i!(n - i)!	
				n_exc = factorial(n);
				k_exc = factorial(i);
				n_k_exc = factorial(n-i);
						
				
				float choose_result = n_exc /(k_exc * n_k_exc);
				
				float power_answer = pow((1-t) , n-i);
				
				float t_to_i = pow(t,i);
				if(t == 0.00)
				{
					t_to_i = 1;
				}
				float temp_point_i_x = ClickX[i];
				float temp_point_i_y = ClickY[i];
				
				//cerr << "INSIDE ... " << ClickX[i] << ' ' << ClickY[i] << endl;
				
				temp_point_i_x = choose_result * power_answer * t_to_i * temp_point_i_x;
				temp_point_i_y = choose_result * power_answer * t_to_i * temp_point_i_y;
				
				if(i == 0)
				{
					 point_i_x = temp_point_i_x;
					 point_i_y = temp_point_i_y;
				}
				else
				{
					point_i_x = point_i_x + temp_point_i_x;
					point_i_y = point_i_y + temp_point_i_y;
				}
			}
			//glVertex2f(point_i_x, point_i_y);
			if(first_time )
			{
				first_time = false;
			}
			else
			{			
				glClickX.push_back(point_i_x);
				glClickY.push_back(point_i_y);
			}
			//cerr << glClickX[0] << ' ' << glClickY[0] << endl;
		
			
			for(unsigned j = 1; j < glClickX.size();j++)
			{
				glVertex2f(glClickX[j-1], glClickY[j-1]);
				glVertex2f(glClickX[j], glClickY[j]);
			}
			//cerr << glClickX[0] << ' ' << glClickY[0] << endl;
		}
		//glVertex2f(ClickX[ClickX.size() - 1 ] , ClickY[ClickY.size() - 1 ] );
	}	 			
	
    glEnd();
    glFlush();
}

void GL_mouse(int button,int state,int x,int y)
{
    y=WINDOW_HEIGHT-y;
    GLdouble mv_mat[16];
    GLdouble proj_mat[16];
    GLint vp_mat[4];
    glGetDoublev(GL_MODELVIEW_MATRIX,mv_mat);
    glGetDoublev(GL_PROJECTION_MATRIX,proj_mat);
    glGetIntegerv(GL_VIEWPORT,vp_mat);

    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        double px,py,dummy_z; // we don't care about the z-value but need something to pass in
        gluUnProject(x,y,0,mv_mat,proj_mat,vp_mat,&px,&py,&dummy_z);
        glutPostRedisplay();
        
        if((ClickX.size() == 0 ) || (ClickX[ClickX.size()-1] != px || ClickY[ClickY.size()-1] != py) )
        {
			ClickX.push_back(px);
			ClickY.push_back(py);
			clicked = true;
		}
        
    }
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
    glutInit(argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //glMatrixMode(GL_PROJECTION_MATRIX);
    //glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    glutCreateWindow("CS 130 - <Insert Name Here>");
    glutDisplayFunc(GL_render);
    glutMouseFunc(GL_mouse);
}

int main(int argc, char** argv)
{
    GLInit(&argc, argv);
    glutMainLoop();
    return 0;
}
