#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "vec.h"
using namespace std;

//Some defines
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;

const int XMIN = -SCREEN_WIDTH/2;
const int XMAX = SCREEN_WIDTH/2;
const int YMIN = -SCREEN_HEIGHT/2;
const int YMAX = SCREEN_HEIGHT/2;

//Plots a point
void plot(int x, int y, float r, float g, float b)
{
    glBegin(GL_POINTS);
    glColor3f(r,g,b);
    glVertex2i(x,y);
    glEnd();
}

//Main Function
int main(int nArgs, char** args)
{
    //Initialize the window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //8 bits for red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); //8 bits for green
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); //8 bits for blue
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //enable page flipping
    SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,SDL_OPENGL);
    SDL_WM_SetCaption("CS130 Lab", NULL);

    //Set up the projection - don't worry about this
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(XMIN, XMAX, YMIN, YMAX,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 spherePosition(0,0,30);
    float sphereRadius=10;
    vec3 lightPosition(-20,-10,-10);
    vec3 pixelgridCenter(0,0,5);
    vec3 cameraPos(0,0,0);
    vec3 ambientColor(.2f,.2f,.2f);
    vec3 diffuseColor(.5f,.5f,.5f);
    vec3 specularColor(.5f,.5f,.5f);
    vec3 x_incr(.025,0,0);
    vec3 y_incr(0,.025,0);

// 	cerr << "X "<< XMIN << ' ' << XMAX << endl;
// 	cerr << "Y "<< YMIN << ' ' << YMAX << endl;
	
	vec3 CP_SP = cameraPos - spherePosition;
	//CP_SP = CP_SP.normalized();
	float dotCPSP = dot(CP_SP, CP_SP);
    while(true)
    {
        //Update platform specific stuff
        SDL_Event event;
        SDL_PollEvent(&event); //Get events
        if(event.type == SDL_QUIT) //if you click the [X] exit out
            break;
        //Graphics
        glClear(GL_COLOR_BUFFER_BIT); //Clear the screen
        
        //dfines the grid the camera looks through
        for(int x = XMIN; x <= XMAX; ++x)
		{
			vec3 pixelgridtemps = pixelgridCenter + x_incr * x;
            for(int y = YMIN; y <= YMAX; ++y)
            {
				vec3 color(0,0,0); // fill this in with the appropriate colors
				
				vec3 pixelgridtemp = pixelgridtemps + y_incr * y;
				
				//shoot ray from camera to pixel grid			
				vec3 rayDirection =  pixelgridtemp - cameraPos;
				rayDirection = rayDirection.normalized();
				
				//vec3 p = t * rayDirection + cameraPos; 
				
				float a = dot(rayDirection, rayDirection);
				
				float b = 2.00 * dot(rayDirection , CP_SP);
				float c = dotCPSP - (sphereRadius * sphereRadius);
			
				if(sqrt(b * b - 4 * a * c) > 0 )
				{
					//cerr << sqrt(b * b - 4 * a * c) << ' ';
					float tplus  =  (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
					float tminus =  (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
					if(tplus > 0 || tminus > 0)
					{
						color = ambientColor + diffuseColor + specularColor;
					}
					
				}
				
				/*
				//1st check if X and Y is at sphere
				if( . . . )
				{
					
					//do ray tracing from point to light to see if it can see the light
									
					color = ambientColor + SOMETHIN_TIMES * diffuseColor + specularColor
				}*/
								
				
								
                plot(x,y,color[0],color[1],color[2]);
                // to help with the math:
                // http://www.csee.umbc.edu/~olano/435f02/ray-sphere.html
            }
		}

        SDL_GL_SwapBuffers(); //Finished drawing
    }



    //Exit out
    SDL_Quit();
    return 0;
}
