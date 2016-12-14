#include "plane.h"
#include "ray.h"
#include <cfloat>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    /*
    u is ray.endpoint
    v is ray.direction
    n is normal
    p is x1
    */
    double raydotn = dot(ray.direction, normal);
    Hit temp;
    if(dot((ray.endpoint - x1), normal) <= 0)
    {
		if(raydotn < 0)
		{
//std::cerr << "RAY Surface TO INSIDE \n";
		   //ray starts on surface of half plane and never leaves 
		   //first hit point
		   temp.object = this;
		   temp.t = 0;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   //second hit point
		   temp.object = this;
		   temp.t = DBL_MAX;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   return true;
		}
		else if(raydotn > 0)
		{
//std::cerr << "RAY Inside to OUTSIDE  \n";
		   //ray starts inside half plane but leaves
		   //first hit point
		   temp.object = this;
		   temp.t = 0;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   //second hit point
		   temp.object = this;
		   temp.t = - dot((ray.endpoint - x1), normal) / raydotn ;
		   temp.ray_exiting = true;
		   hits.push_back(temp);
		   
		   return true;
			
		}
		else
		{
		   //ray has no intersections with surface of plane, 
		   // ray starts inside half space and never leaves 
		   //first hit point
//std::cerr << "RAY inside TO INSIDE \n";
		   temp.object = this;
		   temp.t = 0;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   //second hit point
		   temp.object = this;
		   temp.t = DBL_MAX;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   return true;
		}	
		
	}
	else
	{
		if(raydotn < 0)
		{
//std::cerr << "RAY OUTSIDE TO INSIDE \n";
		   //one intersection, ray starts outside plane 
		   //first hit point
		   temp.object = this;
		   temp.t = - dot((ray.endpoint - x1), normal) / raydotn ;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   //second hit point
		   temp.object = this;
		   temp.t = DBL_MAX;
		   temp.ray_exiting = false;
		   hits.push_back(temp);
		   
		   return true;
		}
		else if(raydotn > 0)
		{
			//doesnt intersect with plane
			return false;
		}
		else
		{
			//doesnt intersect with plane
			return false;
		}	
	}
    
    return false;
}

vec3 Plane::
Normal(const vec3& point, int part) const
{
    return normal;
}
