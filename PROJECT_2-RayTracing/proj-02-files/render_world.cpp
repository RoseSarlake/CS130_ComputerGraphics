#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <iostream>

#include <typeinfo>
extern int width;
extern int height;	

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(Ray& ray, Hit& hit)
{
//std::cerr << ray.direction[0] << ' ' << ray.direction[1] << ' ' << ray.direction[2] << '\n';
    // TODO
    float current_closest_intersection = -1;
    int Index = -1;
    std::vector<Hit> cur_low; 
    for(unsigned int i = 0; i < this->objects.size(); i++)
    {
		std::vector<Hit> temp_low; 
		//if there is hits go into for loop
		//hits of current object from current ray stored in temp_low
		if(this->objects[i]->Intersection(ray,temp_low))
		{			
			//if first time there are hits
			if(current_closest_intersection == -1)
			{
				//check if its a valid hit
				if(temp_low[0].t > small_t)
				{
					cur_low = temp_low;
					current_closest_intersection = temp_low[0].t;
					Index = i;
					
				}
				else if(temp_low[1].t > small_t)
				{
					
					cur_low = temp_low;
					current_closest_intersection = temp_low[0].t;
					Index = i;
				}
			}
			else
			{	
				//find the closest intersection of closest hit 
				//if distance is 0 ignore
				double current_dis = temp_low[0].t;
				if(temp_low[0].t <= small_t)  
				{
					current_dis = temp_low[1].t;
				}
				
				//if current_dis is closer than closest_dis 
				//then make  closest_dis = current_dis
				if(current_dis < current_closest_intersection || current_closest_intersection < small_t)
				{
					cur_low = temp_low;
					current_closest_intersection = current_dis;
					Index = i;
					
				}

			}
		}
	}

	// return closest object if there was an intersection
	if(current_closest_intersection > small_t)
	{
		if(cur_low[0].t <= small_t )
		{
		   hit = cur_low[1];
		}
		else
		{
			hit = cur_low[0];
		}
//std::cerr << cur_low[0].t <<' ' << cur_low[1].t << ' ' << hit.t << " = " << current_closest_intersection << '\n';
		return this->objects[Index];
	}
	
	// return null if no intersections found
    return 0;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray; // TODO: set up the initial view ray here
    //ray set endpoint and direction vector
    //endpoint should be ... camera location
    //direction vector is the focal point of the camera is image plane
    //pixel_index[0] = x  pixel_index[1] = y ... 
    //so... new vec3 and input pixel index 
    vec3 direction;
	direction = this->camera.World_Position(pixel_index);
	
    ray.endpoint = this->camera.position;
    ray.direction = direction.normalized();
    vec3 color = Cast_Ray(ray,1);
    camera.film.Set_Pixel(pixel_index,Pixel_Color(color));
/*
if(pixel_index[0] == 0 && pixel_index[1] == 0)
{
	Hit hit; 
	Object* object; 
	object = Closest_Intersection(ray,hit);
	vec3 Intersection = ray.direction * hit.t + ray.endpoint;	
std::cerr << ray.direction << '\n';
}*/

}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(Ray& ray,int recursion_depth)
{
    // TODO
    vec3 color;
    
    Hit hit;
	Object* ClosestObj = this->Closest_Intersection(ray,hit);
	
    // determine the color here
	if(ClosestObj != 0)
	{				
		//we have direction and we have endpoint and we have distance we 
		//travel the ray to hit intersection
		/*
		double D = ray.direction.magnitude();
		D = hit.t / D;
		*/
		vec3 intersection_point = ray.endpoint + ray.direction * hit.t;
//std::cerr << typeid(*ClosestObj).name() << '\n';		
		vec3 normal;
		if(hit.ray_exiting)
		{
			normal = -hit.object->Normal( intersection_point, hit.part);
		}
		else
		{ 
			normal = hit.object->Normal( intersection_point, hit.part);
		}
		//color = ClosestObj->material_shader->Shade_Surface(ray, intersection_point, ClosestObj->Normal( intersection_point, hit.part), recursion_depth );
		color = ClosestObj->material_shader->Shade_Surface(ray, intersection_point, normal, recursion_depth );
	}
	else
	{
		color = this->background_shader->Shade_Surface(ray,ray.endpoint,ray.endpoint,this->recursion_depth_limit);
	}
    return color;
}
