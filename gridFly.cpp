#include "gridFly.h"

#include <GL/gl.h>

using namespace std;

const float pi = 3.14159265359f;
const float to_rad = pi/180.0f;

void GridFly::update(float dt)
{
	bool moved = false;
	float mspeed = speed;
	
#if 1
	//increase wasd speed with shift
	if (jeltz->button("Left Ctrl"))
		mspeed *= 0.1;
	if (jeltz->button("Left Shift"))
		mspeed *= 10.0;
		
	//standard wasd fly movement
	if (jeltz->button("A"))
		{pos.x += mspeed * dt; moved = true;}
	if (jeltz->button("D"))
		{pos.x += mspeed * -dt; moved = true;}
	if (jeltz->button("S"))
		{pos.z += mspeed * dt; moved = true;}
	if (jeltz->button("W"))
		{pos.z += mspeed * -dt; moved = true;}
	
	//middle mouse pan
	/*if (jeltz->button("MButton"))
	{
		vec2i move = jeltz->mouseMove();
		if (move.x != 0 || move.y != 0)
		{
			float msensitivity = 1.0f;
			if (jeltz->button("Left Ctrl"))
				msensitivity *= 0.1f;
			if (jeltz->button("Left Shift"))
				msensitivity *= 10.0f;
			vec2f dir;
			if (camera.getZoom() > 0.1f)
				dir = vec2f(tan(camera.getFOVX()*0.5f), tan(camera.getFOVY()*0.5f)) * (2.0f * camera.getZoom() * msensitivity) * vec2f(-move.x, move.y) / jeltz->winSize();
			else
				dir = vec2f(-move.x, move.y) * msensitivity * mspeed * sensitivity * 0.01f;
			camera.move(camera.upVec() * dir.y + camera.rightVec() * dir.x);
			moved = true;
		}
	}*/
	
	//right mouse zoom
	if (jeltz->button("RButton"))
	{
		vec2i move = jeltz->mouseMove();
		if (move.y != 0)
		{
			float msensitivity = sensitivity * speed * 0.05;
			if (jeltz->button("Left Ctrl"))
				msensitivity *= 0.1;
			if (jeltz->button("Left Shift"))
				msensitivity *= 10.0;
			
			//camera.zoom(-move.y * (camera.getZoom() + 1.0) * msensitivity);
			//if (camera.getZoom() == 0.0 && move.y > 0.0 && jeltz->button("Left Shift"))
			//	camera.move(camera.toVec() * 0.1f);
			moved = true;
		}
	}
#endif

	camera.zoomAt(pos, vec3f(pos.x, 0, pos.z));
	
	//only regen matrix if the camera has changed
	if (moved)
		camera.regenCamera();
		
	//update projection aspect ratio if window resized
	if (jeltz->resized())
	{
		this->camera.setAspectRatio(jeltz->winSize().x/(float)jeltz->winSize().y);
		this->camera.regenProjection();
	}
}




GridFly::GridFly()
{
	camera.setPerspective(90.0f * (pi/180.0f));
	camera.setDistance(0.01f, 100.0f);
	camera.regen();

	sensitivity = 0.5;
	speed = 100.0;
	pos = vec3f(50, -20, 50);
	
	camera.zoomAt(pos, vec3f(pos.x, 0, pos.z));
	camera.regenCamera();
}

GridFly::~GridFly()
{
}

void GridFly::uploadCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.getProjection().m);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.getInverse().m);
}

vec3f GridFly::getMousePos(const vec2f &mousePosN)
{
	vec3f start;
	vec3f end;

	camera.projectMouse(mousePosN, start, end);
	
	vec3f n(0, 1, 0);
	vec3f dir = end - start;

	float d = (-start).dot(n) / dir.dot(n);

	dir *= d;

	return start + dir;
}

