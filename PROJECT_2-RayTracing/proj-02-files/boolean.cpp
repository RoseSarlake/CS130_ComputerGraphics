#include "boolean.h"

// Determine if the ray intersects with the boolean of A and B.
bool Boolean::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    if(type == type_intersection)
    {
		std::vector<Hit> hits1;
		std::vector<Hit> hits2;	
		if(A->Intersection(ray,hits1)&& B->Intersection(ray,hits2))
		{
			float A1 = hits1[0].t;
			float A2 = hits1[1].t;
			float B1 = hits2[0].t;
			float B2 = hits2[1].t;
			if( A2 < B1 )
			{
				return false;
			}
			else if( B2 < A1 )
			{
				return false;
			}
			else if( A1 <= B1 && B2 <= A2 )
			{
				
				hits = hits2;
				return true;
			}
			else if( B1 <= A1 && A2 <= B2 )
			{
				hits = hits1;
				return true;
			}
			else if( A1 <= B1 && A2 <= B2 )
			{
				hits.push_back(hits2[0]);
				hits.push_back(hits1[1]);
				return true;
			}
			else if( B1 <= A1 && B2 <= A2 )
			{
				hits.push_back(hits1[0]);
				hits.push_back(hits2[1]);
				return true;
			}
			
			
		}
		return false;
		
	}
	else if(type == type_difference)
	{
		std::vector<Hit> hits1;
		std::vector<Hit> hits2;
		if(A->Intersection(ray,hits1))
		{
			if( B->Intersection(ray,hits2) )
			{
				float A1 = hits1[0].t;
				float A2 = hits1[1].t;
				float B1 = hits2[0].t;
				float B2 = hits2[1].t;
				if( A2 < B1 )
				{
					//std::cerr << "IN1 ";
					hits = hits1;
					
					//*B = *this;
					return true;
				}
				else if( B2 < A1 )
				{
					
					//std::cerr << "IN2 ";
					hits = hits1;
					return true;
				}
				else if( A1 < B1 && B1 < A2 && A2 < B2 )
				{
					//std::cerr << "IN3 ";
					
					hits.push_back( hits1[0] );
					hits.push_back( hits2[0] );
					//*B = *this;
					return true;
				}  
				else if( B1 < A1 && B2 < A2 && A1 < B2 )
				{
					//std::cerr << "IN4 ";
					
					hits.push_back( hits2[1] );
					hits.push_back( hits1[1] );
					//*B = *this;
					return true;
				}
				else if( B1 < A1 && A2 < B2  )
				{
					return false;
				}	
								
			}
			hits = hits1;
			return true;
		}
		
		return false;
	}
	else if(type == type_union)
	{
		std::vector<Hit> hits1;
		std::vector<Hit> hits2;
		bool objA = A->Intersection(ray,hits1);
		bool objB = B->Intersection(ray,hits2);
		//if ray intersects both in union
		if(objA && objB)
		{
			if(hits1[0].t < hits2[0].t)
			{
				hits.push_back(hits1[0]);
			}
			else
			{
				hits.push_back(hits2[0]);
			}
			
			if(hits1[1].t > hits2[1].t)
			{
				hits.push_back(hits1[1]);
			}
			else
			{
				hits.push_back(hits2[1]);
			}
			return true;
		}
		//if ray intersects only A
		else if(objA)
		{
			hits = hits1;
			return true;
		}
		//if ray intersects only B
		else if(objB)
		{
			hits = hits2;
			return true;
			
		}
		return false;
	}
    return false;
}

// This should never be called.
vec3 Boolean::Normal(const vec3& point, int part) const
{
    assert(false);
    return vec3();
}
