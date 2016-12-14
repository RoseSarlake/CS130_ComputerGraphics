#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth) const
{
	vec3 color;
    vec3 colorA;
	colorA = colorA + color_ambient * this->world.ambient_color * this->world.ambient_intensity;
    vec3 colorD;
    vec3 colorS;
    // TODO: determine the color
    //phong shder equation
    //I = RaLa + RdL max(0,l.n) + RsL max(0,v.r)^alapha
    //n = same_side_normal	*
	//l = ray from intersection to light.position --> light.position - intersection
	//v = ray from intersection to ray.endpoint --> endpoint -intersection
	
	//p = intersection point	*
	//Ra = color_ambient	*
	//Rd = color_diffuse	*
	//Rs = color_specular	*
	//alpha = specular_power	*
	//l
    
    //point source 	LightOriginal /	mag|p-p0| ^2 
    for(unsigned i = 0; i < this->world.lights.size(); i++ )
    {
		//used to temperarily hold vectors needed
		vec3 temp;
		
		//vec3 Ra = color_ambient;
		vec3 Rd = color_diffuse;
		vec3 Rs = color_specular;
		
		//vec3 La = this->world.lights[i]->color;
		
		//get L not divded by distance yet
		vec3 L = this->world.lights[i]->Emitted_Light(ray);
	
		// find the vector light source to intersection point 
		temp = intersection_point -  this->world.lights[i]->position;
		//square the magnitude of the vector
		float dis_L_Ipt = temp.magnitude_squared();
		//divide light intensity by distance squared
		L  = L / dis_L_Ipt;
		
		vec3 v = intersection_point - ray.endpoint;
		v = v.normalized();
		
		vec3 n = same_side_normal;
		n = n.normalized();
		vec3 l = this->world.lights[i]->position - intersection_point;
		l = l.normalized();
		//n need to be normalized ** maybe l need to be normalized?
		vec3 r = l.normalized() - n.normalized() * (2 * dot(l.normalized(),n.normalized()));
		r = r.normalized();
		//max(0,l.n)
		float ldotn = dot(l,n);
		
		if(ldotn < 0)
		{
			ldotn = 0;
		}

		
		//max(0,v.r)
		float vdotr = dot(v,r);
		if(vdotr < 0)
		{
			vdotr = 0;
		}
		
		//check if shadow enabled
		if(this->world.enable_shadows)
		{
		
		
			//Draw a ray from intersection point to light
			Ray Int_to_light;
			Int_to_light.endpoint = intersection_point;
			Int_to_light.direction =  this->world.lights[i]->position - intersection_point;
			float Dis_int_light = Int_to_light.direction.magnitude();
			Int_to_light.direction = Int_to_light.direction.normalized();
			
			
			//this will return the closest hit or 0
			Hit hit;
			hit.t = 0;
			
			Object* FirstObj = this->world.Closest_Intersection(Int_to_light,hit);
			
			//find intersection point of object
			vec3 NewPt = Int_to_light.endpoint + Int_to_light.direction * hit.t;
			NewPt = NewPt - Int_to_light.endpoint;
			float NewPtDis = NewPt.magnitude();



			if(FirstObj != 0 && Dis_int_light > NewPtDis  ) 
			{
			}
			else
			{
				colorD = colorD + Rd * L * ldotn;
				colorS = colorS + Rs * L * pow(vdotr, specular_power); 
			}
		
		// so now I have a ray if there is an object between pt and light --> shadow
			//how to check... first if no hits then good
		//			so use an if statement and not do Diffuse and Specular if true
		//no hit
			
	    }
	    else
	    {
		
			colorD = colorD + Rd * L * ldotn;
			colorS = colorS + Rs * L * pow(vdotr, specular_power); 
		}
		
		
	}
		color = colorD + colorA + colorS;
    
    
    return color;
}
