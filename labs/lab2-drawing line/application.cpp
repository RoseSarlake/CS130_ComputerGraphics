#include "application.h"

#include <iostream>
#include <cassert>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// set pixel (x,y) in framebuffer to color col, where
// col is a float array of three values between 0 and 1
// which specify the amount of red, green, and blue to mix (e.g.
// RED: (1,0,0) GREEN: (0,1,0) BLUE: (0,0,1)
// YELLOW: (1,1,0) MAGENTA: (1,0,1) CYAN: (0,1,1)
// )
//
void set_pixel(int x, int y, float col[3])
{
    // write a 1x1 block of pixels of color col to framebuffer
    // coordinates (x, y)
    glRasterPos2i(x, y);
    glDrawPixels(1, 1, GL_RGB, GL_FLOAT, col);
}

void set_pixel(int x, int y)
{
    float col[] = { 1.0, 1.0, 1.0 };
    set_pixel(x,y,col);
}


//
// This is where the line drawing algorithm gets called
// Fix it so that there are no gaps and that it works for all cases
// This lab requires you to use dda to render the line:
// http://en.wikipedia.org/wiki/Digital_differential_analyzer_%28graphics_algorithm%29
//
void draw_line(int x0, int y0, int x1, int y1, float col[3])
{
    //NOT WORKING CODE(PUT BETTER CODE HERE!!)
    float dx = x1 - x0;
    float dy = y1 - y0;

    float m = 0;
    
    if(dx != 0 )
    {
		m = dy/dx;
	}
    
    float b = y0 - m*x0;
    
    //m_abs will hold the absolute value of slope
    // is_neg is false unless m is negative
    float m_abs;
    bool is_neg = false;
    if(m < 0)
    {
		m_abs = -m;
		is_neg = true;
	}
	else
	{
		m_abs = m;
	}
    
    
    if( m_abs < 1 && m_abs > 0)
    {
		if(x1 > x0 )
		{
			if(!is_neg) 
			{ 
				for(int x = x0; x < x1; ++x)
				{	
					set_pixel(x, m_abs*x + b);
				}	
				
			}
			else
			{
				for(int x = x0; x < x1; ++x)
				{	
					set_pixel(x, m*x + b);
				}
			}
		}
		else
		{
			
			if(!is_neg) 
			{ 
				for(int x = x1; x < x0; ++x)
				{	
					set_pixel(x, m_abs*x + b);
				}	
				
			}
			else
			{
				for(int x = x1; x < x0; ++x)
				{	
					set_pixel(x, m*x + b);
				}
			}
			
		}
		
		
	}
	else if( m_abs > 1 )
	{
		float first_half = m_abs;
		if(x1 > x0 )
		{
			if(!is_neg)
			{
				for(int x = x0; x<x1; x++)
				{
					set_pixel(x, m_abs*x + b);
					for(int y = 0; y <= first_half; y++)
					{
						set_pixel(x, m_abs*x+b+y);
					}
					
				}
			}
			else
			{
				for(int x = x0; x<x1; x++)
				{
					set_pixel(x, m_abs*x + b);
					for(int i = 0; i <= first_half; i++)
					{
						set_pixel(x, m*x+b-i);
					}
					
				}
			}
		}
		else
		{
			if(!is_neg)
			{
				for(int x = x1; x < x0; x++)
				{
					set_pixel(x, m_abs*x + b);
					for(int y = 0; y <= first_half; y++)
					{
						set_pixel(x, m_abs*x+b+y);
					}
					
				}
			}
			else
			{
				for(int x = x1; x < x0; x++)
				{
					set_pixel(x, m_abs*x + b);
					for(int i = 0; i <= first_half; i++)
					{
						set_pixel(x, m*x+b-i);
					}
					
				}
			}
		}
	}
	else if( m == 0 )
	{
		if(x1 == x0)
		{
			if(y1 > y0)
			{
				for(int y = y0; y < y1; ++y)
				{	
					set_pixel( x0 , y );
				}
			}
			else
			{
				for(int y = y1; y < y0; ++y)
				{	
					set_pixel( x0 , y );
				}
			}
		}
		else
		{
			if(x1 > x0)
			{
				for(int x = x0; x < x1; ++x)
				{	
					set_pixel( x , y0 );
				}
			}
			else
			{
				for(int x = x1; x < x0; ++x)
				{	
					set_pixel( x , y0 );
				}
			}
		}
	
	}
			
}

// triggered each time the application needs to redraw
void application::draw_event()
{
    //
    // This code draws a single red horizontal line across
    // the the middle of the window using the function
    // set_pixel. Replace this code with your DDA
    // implementation that can draw a line of any slope. You
    // should only need to call set_pixel within xd your
    // routine.
    //
    float red[] = { 1.0, 0.0, 0.0 };
    float white[] = { 1.0, 1.0, 1.0 };
    for (int i = 0; i < this->width; ++i)
    {
        set_pixel(i, (int)this->height/2, red);
    }

    //
    // This code will draw each of the lines
    //
    for (size_t i = 0; i < lines.size(); ++i)
    {
    draw_line(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1, white);
    }
}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
    // Each time the mouse button is pushed down, add a new
    // line segment to our list.
    if (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN) {
        lines.push_back(line_segment(x, y, x, y));
        button_down = true;
    }
    if (button == GLUT_LEFT_BUTTON && button_state == GLUT_UP) {
        button_down = false;
    }
    //cout << "button event: " << button << " " << button_state
    //    << " " << x << " " << y << endl;
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
    // While the mouse button is down, update the last
    // endpoint in our list to reflect the current location
    // of the mouse.
    lines[lines.size()-1].x1 = x;
    lines[lines.size()-1].y1 = y;

    //cout << "move event: " << x << " " << y << endl;
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
    // clear our stored clicks
    if (key == 'c')
        lines.clear();

    //cout << "keyboard event: " << key << " "
    //    << x << " " << y << endl;
}
