#include <iostream>
#include <iomanip>
#include "vec.h"

using namespace std;

void Dump_png(unsigned int* data,int width,int height,const char* filename);

/*
  Problems 1-4

  Rewrite each loop so that only addition and subtraction occur inside any loop.
  This includes operations performed on the vec3 vectors.  Ignore the operations
  that occur while printing the results.

  Each problem is worth 10 points.  Note that for Problem 4, you must clear the
  multiplications and divisions from both loops.  If you are able to clear the
  multiplications and divisions from only the inner loop, you will still get 5
  points.
*/

// DONE
void Problem_1(int n, float x, float y)
{
    int i = -n;
    float z = i * x + y;
    cout << z << endl;
    
    while(i <= n )
    {
        z = (z-y) + x + y;
        cout << z << endl;
        i++;
    }
}

// DONE
void Problem_2(int n, float x, float dx)
{
    //answer
    int i = 0; 
    float y = x;
    float z = y * y;
    
    //constants 
    float z_naut = y * y;
    float two_dx_squared = 2 * dx * dx;
    float two_y_dx = 2 * y * dx;
    
    //mutables
    float adding_dx_squared = dx * dx;
    float num_dx = adding_dx_squared;
    
    float num_y_dx = 0;
    
    cout << z << endl;
    i++;
    
    while( i < n )
    {
        // add 2ydx to num_y_dx b/c the ydx portion increases by 2ydx everytime
        num_y_dx = num_y_dx + two_y_dx;
        
        //get z by adding constant z^2 + __ydx + __dx^2
        z = z_naut + num_y_dx + num_dx;
        
        //add 2dx^2 to currently adding_dx_squared, ex: 1 --> 3 --> 5
        adding_dx_squared = two_dx_squared + adding_dx_squared;
        
        // add adding_dx_squared to num_dx to get next z value
        num_dx = num_dx + adding_dx_squared;
        
        cout << z << endl;
        i++;
    }
}

//DONE
void Problem_3(int n, vec3 u, vec3 v, vec3 du)
{
    // solved
    float du_dot_v = dot(du,v);
    float temp_two = dot(u,v);
    float v_squared = dot(v,v);
    float dot_div = du_dot_v / v_squared;
    float z = temp_two/v_squared;
    for(int i = 0 ; i < n ; i++)
    {
        cout << z << endl;
        z = z + dot_div;
        
    }
    
}

void Problem_4(int n, vec3 w, vec3 u, vec3 v)
{
    
    //const
    float one_over_n = (float)1/n;
    vec3 u_over_n = u * one_over_n;
    vec3 w_over_n = w * one_over_n;
    vec3 v_over_n = v * one_over_n;
    
    //mutables
    vec3 temp_w;
    vec3 temp_v = v;
    vec3 temp_v_in_one;
    
    for(int i = 0 ; i <= n ; i++)
    {
        
        if(i > 0)
        {
            temp_w = temp_w + w_over_n;
        }
        
        if(i > 0)
        {
            temp_v = temp_v - v_over_n;
        }

        vec3 temp_u;
        temp_v_in_one = temp_v;
        bool first_time = true;
        for(int j = 0 ; i + j <= n ; j++)
        {
            if(j > 0)
            {
                temp_u = temp_u + u_over_n;
            }
            
            if(!first_time)
            {
                temp_v = temp_v - v_over_n;
            }
            
            //temp_a * w    temp_w          temp_v
            vec3 zee = temp_w  + temp_u + temp_v;
            cout << zee << endl;
            first_time = false;
        }
        temp_v = temp_v_in_one;
    }
}

/*
  Problem 5

  Draw a white circle centered at (x,y) with radius r.
  For example, data[y][x]=0xffffffff; would set the center of the circle to white.
  The circle should not have gaps.
  You may assume that the circle does not cross the edges of the circle.

  20 points.

  Extra credit options:
  A = you don't use any floating point.
  B = you do not use * or / in any loop, including in the loop condition.
  C = you fill the circle red (0xff0000ff) while keeping the outline white.

  A = 2% extra credit
  A + B = 5% extra credit
  A + B + C = 10% extra credit
*/
void draw_circle(int x, int y, int r)
{
    unsigned int data[512][512];
    for(int i = 0 ; i < 512; i++)
        for(int j = 0 ; j < 512; j++)
            data[i][j] = 0xff000000; // set all pixels to (0,0,0,1) = black.

    // TODO: your code here
    int r_squared = r*r;
    float g;
    //determine which points are in a circle
	for(int i = x; i <= (x+r); i++)
	{
		for(int j = y; j<= (y+r); j++)
		{
			int temp_t = (i-x)*(i-x);
			int temp_v = (j-y)*(j-y);
			if( (temp_t + temp_v) <= r_squared)
			{			
				data[j][i]=0xffffffff;
			}
			else
			{
			    g = i-x+.5;
			    g = g*g;
			    g = g+temp_v;
			    if(g <= r_squared)
			    {
			        data[j][i]=0xffffffff;
			    }
			}
		}
	}
	
	for(int i = x; i <= (x+r); i++)
	{
		for(int j = y; j >= (y-r); j--)
		{
			int temp_t = (i-x)*(i-x);
			int temp_v = (j-y)*(j-y);
			if( (temp_t + temp_v) <= r_squared)
			{			
				data[j][i]=0xffffffff;
			}
			else
			{
			    g = i-x-.5;
			    g = g*g;
			    g = g+temp_v;
			    if(g <= r_squared)
			    {
			        data[j][i]=0xffffffff;
			    }
			}
			
		}
	}
	
	for(int i = x; i >= (x-r); i--)
	{
		for(int j = y; j >= (y-r); j--)
		{
			int temp_t = (i-x)*(i-x);
			int temp_v = (j-y)*(j-y);
			if( (temp_t + temp_v) <= r_squared)
			{			
				data[j][i]=0xffffffff;
			}
			else
			{
			    g = i-x-.5;
			    g = g*g;
			    g = g+temp_v;
			    if(g <= r_squared)
			    {
			        data[j][i]=0xffffffff;
			    }
			}
		}
	}
	
	for(int i = x; i >= (x-r); i--)
	{
		for(int j = y; j <= (y + r); j++)
		{
			int temp_t = (i-x)*(i-x);
			int temp_v = (j-y)*(j-y);
			if( (temp_t + temp_v) <= r_squared)
			{			
				data[j][i]=0xffffffff;
			}
			else
			{
			    g = i-x+.5;
			    g = g*g;
			    g = g+temp_v;
			    if(g <= r_squared)
			    {
			        data[j][i]=0xffffffff;
			    }
			}
			
		}
	}
	
    Dump_png(&data[0][0],512,512,"circle.png");
}

int main()
{
    cout << std::setprecision(4);

    cout << "== Problem 1 ==" << endl;
    
    Problem_1(5, -0.123, 2.114);

    cout << "== Problem 2 ==" << endl;

    Problem_2(5, -0.123, 2.114);

    cout << "== Problem 3 ==" << endl;

    Problem_3(5, vec3(.1,.2,.3), vec3(0.9,0.2,0.8), vec3(0.0,0.1,0.6));

    cout << "== Problem 4 ==" << endl;

    Problem_4(5, vec3(.1,.2,.3), vec3(0.9,0.2,0.8), vec3(0.0,0.1,0.6));

    cout << "== Problem 5 ==" << endl;

    draw_circle(220, 260, 200);

    return 0;
}

