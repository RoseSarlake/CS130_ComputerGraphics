#include "cylinder.h"
#include "ray.h"
#include <algorithm>
#include <cfloat>

// Determine if the ray intersects with the cylinder.
bool Cylinder::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    /*
     f(t) = u + tv <-- ray
     u = ray.endpoint
     v = ray.direction.normalized()
     r = radius
     p = A
     q = B
     y = u - p 
     w = q - p 
     w_hat = w / w.magnitude()
     v_bar = v cross w_hat
     y_bar = y cross w_hat
      
     */
     float hitA1 = 0;
     float hitA2 = 0;
     float hitB1 = 0;
     float hitB2 = 0;
     
     int partA = 1;
     int partB1 = 0;
     int partB2 = 0;

     
     
	vec3 u = ray.endpoint;
    vec3 v = ray.direction.normalized();
    double r = radius;
    vec3 p = A;
    vec3 q = B;
    vec3 y = u - p;
    vec3 w = q - p;
    vec3 w_hat = w / w.magnitude();
    vec3 v_bar = cross(v, w_hat);
    vec3 y_bar = cross(y, w_hat);
    vec3 n = p-q / (p-q).magnitude();
    
    if(dot(v_bar,v_bar) == 0 )
    {
		//u get hit (0,inf)
		if(r >= y_bar.magnitude())
		{
			hitA1 = 0;
			hitA2 = DBL_MAX;
		}
		else
		{
			return false;
		}
		
	}
	else if(r * v_bar.magnitude() >= cross(v_bar, y_bar).magnitude())
	{
		float temp = sqrt(r*r*dot(v_bar,v_bar) - cross(v_bar,y_bar).magnitude() * cross(v_bar,y_bar).magnitude());
		float t1 = -dot(v_bar,y_bar) - temp;
		float t2 = -dot(v_bar,y_bar) + temp;
		//hit (0,t2)
		if(t1 < 0 && t2 >= 0)
		{
			hitA2 = t2;
		}
		//hit (t1,t2)
		else if(t1 >= 0 && t2 >= 0)
		{		
			hitA1 = t1;
			hitA2 = t2;	
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		return false;
	}
    
    
    if(dot(u-p,n) <= 0)
    {
		if(dot(u-q,n) >= 0)
		{	
			//get (0, inf)
			if(dot(v,n) == 0)
			{
				hitB1 = 0;
				hitB2 = DBL_MAX;	
			}
			//get (0, tp)
			else if(dot(v,n) > 0)
			{
				hitB1 = 0;
				hitB2 = -dot((u-p),n) / dot(v,n);
				partB1 = 0;
				partB2 = 2;
				
			}
			//get (0, tq)
			else
			{
				partB1 = 0;
				partB2 = 3;
				hitB1 = 0;
				hitB2 = -dot((u-q),n) / dot(v,n);				
			}
			
		}
	}
	else if(dot(u-q,n) < 0)
	{
		//get (tq, tp)
		if(dot(v,n) > 0)
		{
			hitB1 = -dot((u-q),n) / dot(v,n);
			hitB2 = -dot((u-p),n) / dot(v,n);
			partB1 = 3;
			partB2 = 2;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//get (tp, tq)
		if(dot(v,n) < 0)
		{
			hitB1 = -dot((u-p),n) / dot(v,n);
			hitB2 = -dot((u-q),n) / dot(v,n);
			partB1 = 2;
			partB2 = 3;
		}
		else
		{
			return false;
		}
	}
	//so we have 4 pts 
	//look at all 4 
	//u know 1 < 2
	//then the hits is A1 A2
	Hit temp;
	if( hitA2 < hitB1 )
	{
		return false;
	}
	else if( hitB2 < hitA1 )
	{
		return false;
	}
	else if( hitA1 <= hitB1 && hitB2 <= hitA2 )
	{
		std::cerr << "IN";
		//hits = hits2;
	    temp.object = this;
	    temp.t = hitB1;
	    temp.part = partB1;
	    temp.ray_exiting = false;
	    hits.push_back(temp);
	   
	    temp.object = this;
	    temp.t = hitB1;
	    temp.part = partB2;
	    temp.ray_exiting = true;
	    hits.push_back(temp);
	   
		return true;
	}
	else if( hitB1 <= hitA1 && hitA2 <= hitB2 )
	{
		//std::cerr << "IN";
		//hits = hits1;
	    temp.object = this;
	    temp.t = hitA1;
	    temp.part = partA;
	    temp.ray_exiting = false;
	    hits.push_back(temp);
	   
	    temp.object = this;
	    temp.t = hitA1;
	    temp.part = partA;
	    temp.ray_exiting = true;
	    hits.push_back(temp);
		return true;
	}
	else if( hitA1 <= hitB1 && hitA2 <= hitB2 )
	{
		//hits.push_back(hits2[0]);
		//hits.push_back(hits1[1]);
		//std::cerr << "IN";
	    temp.object = this;
	    temp.t = hitB1;
	    temp.part = partB1;
	    temp.ray_exiting = false;
	    hits.push_back(temp);
	   
	    temp.object = this;
	    temp.t = hitA2;
	    temp.part = partA;
	    temp.ray_exiting = true;
	    hits.push_back(temp);
		
		return true;
	}
	else if( hitB1 <= hitA1 && hitB2 <= hitA2 )
	{
		//hits.push_back(hits1[0]);
		//hits.push_back(hits2[1]);
		//std::cerr << "IN";
	    temp.object = this;
	    temp.t = hitA1;
	    temp.part = partA;
	    temp.ray_exiting = false;
	    hits.push_back(temp);
	   
	    temp.object = this;
	    temp.t = hitB1;
	    temp.part = partB2;
	    temp.ray_exiting = true;
	    hits.push_back(temp);
		return true;
	}
	
	
    
    return false;
}

// part 1 = side, 2 = A side, 3 = B side
vec3 Cylinder::Normal(const vec3& point, int part) const
{
    vec3 normal;
    // TODO: set the normal
    //find normal of side
    if(part == 1)
    {
		//we know that mag point - X1 is r
		// P - B --> BP , A - B --> BA
		//
		vec3 temp1 = point - this->B;
		vec3 temp2 = this->B - this->A;
		float i = temp1.magnitude_squared();
		float j = temp2[0] * temp1[0] +temp2[1] * temp1[1] +temp2[2] * temp1[2];
		float t = i/j;
		normal = point - ((this->A - this->B).normalized() * t + this->B);		
	}
	//find p normal
	else if(part == 2)
	{
		normal = (this->A - this->B).normalized();
	}
	//find q normal 
	else if(part == 3)
	{
		normal = (this->B - this->A).normalized();
	}
    
    
    
    
    return normal;
}
