#pragma once

#include <pyarlib/jeltz.h>
#include <pyarlib/camera.h>

class GridFly : public JeltzPlugin
{
private:
	virtual void update(float dt);

public:
	float sensitivity;
	float speed;

	vec3f pos;
	Camera camera; //the public camera class. usage eg: glLoadMatrixf(jeltzFly.camera.getInverse().m);

	GridFly();
	virtual ~GridFly();

	void uploadCamera(); //sets deprecated GL projection/modelview matrices

	vec2f getMousePos(const vec2f &mousePosN);
};

