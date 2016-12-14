#include "quaternion.h"
#include "vec.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace std;

quaternion slerp(float u, const quaternion& p, const quaternion& q)
{
	//clamp u to 1 and -1
	if(u > 1)
	{
		u = 1;
	}
	else if(u < -1)
	{
		u = -1;
	}
	
	//qa = p qb = q
	// normalize p and q
	quaternion i = p.normalized();

	quaternion j = q.normalized();

	// what is u? u is t
	//dot p and q to get angle
	float theta = dot(i,j);	
	
	quaternion SLERP = ( p * sin((1-u) * theta) + q * sin(u*theta) )/ sin(theta);
	
    // implement me!
    return SLERP;
}

void quaternion::from_angle_axis(float angle, const vec3& u)
{
    float radians = angle/2 * (M_PI/180);
    s = cos(radians);
    v = sin(radians) * u.normalized();
}

void quaternion::to_angle_axis(float &angle, vec3 &u) const
{
    float radians = 2 * atan2(v.magnitude(), s);
    angle = radians * (180/M_PI);
    u = v.normalized();
}

quaternion pow(const quaternion& q, float r)
{
    float angle;
    vec3 axis;
    q.to_angle_axis(angle, axis);
    quaternion p;
    p.from_angle_axis(angle * r, axis);
    return p;
}
