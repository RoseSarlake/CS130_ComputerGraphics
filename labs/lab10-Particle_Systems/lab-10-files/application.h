#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <map>

#include "gl_texture.h"
#include "gl_viewer.h"
#include "obj.h"
#include "timer.h"

typedef std::map<std::string, gl_image_texture*>
    gl_image_texture_map;

class application : public gl_viewer
{
public:
    application();
    ~application();
    void init_event();
    void draw_event();
    void mouse_click_event(int button, int button_state, int x, int y);
    void mouse_move_event(int x, int y);
    void keyboard_event(unsigned char key, int x, int y);

private:
    bool raytrace;
    int rendmode;
    int npart;
    timer t;

    obj o;
    gl_image_texture_map texs;

    bool paused;
    float sim_t;
		std::vector<vec3> PARTICLES_VECTOR;					//holds current position
	//PARTICLES_VECTOR.resize(120);
	std::vector<vec3> NEXT_PARTICLES_VECTOR;			//holds next position
	//NEXT_PARTICLES_VECTOR.resize(120);
	std::vector<vec3> VELOCITY_VECTOR;					//holds current velocity
	//VELOCITY_VECTOR.resize(120);
	std::vector<vec3> NEXT_VELOCITY_VECTOR;				//holds next velocity
	//NEXT_VELOCITY_VECTOR.resize(120);	
	unsigned int CURRENT_PARTICLE;
	unsigned int MAX_OUTPUTED_PARTICLES;
};

#endif
