#include "camera.h"
extern int width;
extern int height;	

void Camera::Position_And_Aim_Camera(const vec3& position_input,
    const vec3& look_at_point,const vec3& pseudo_up_vector)
{
    position=position_input;
    look_vector=(look_at_point-position).normalized();
    horizontal_vector=cross(look_vector,pseudo_up_vector).normalized();
    vertical_vector=cross(horizontal_vector,look_vector).normalized();
}

void Camera::Focus_Camera(double focal_distance,double aspect_ratio,
    double field_of_view)
{
    focal_point=position+look_vector*focal_distance;
    film.width=(double)2*focal_distance*tan((double).5*field_of_view);
    film.height=film.width/aspect_ratio;
}

// Find the world position of the input pixel
vec3 Camera::World_Position(const ivec2& pixel_index)
{
    vec3 result;
    // TODO
    result = this->focal_point + this->vertical_vector * ((pixel_index[1] - height / 2.0) / height) * film.height;
	result = result + this->horizontal_vector * ((pixel_index[0] - width / 2.0 )/ height) * film.height ;
	result = result - this->position;
	//result = result.normalized();
//std::cerr << "FILM HEIGHT " << film.height << " CAMERA HEIGHT " << height << std::endl;
    return result;
}
