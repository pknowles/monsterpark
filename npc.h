#pragma once

#include <pyarlib/vec.h>
#include <vector>
#include "util.h"

class NPC;

class NPCGroup {
public:
	std::vector<std::vector<NPC*> > all;
	std::vector<std::vector<NPC*> > grid;
	pyarlib::Box gridArea;
	void draw();
	void update(float dt);
	NPC* add(vec2f spawn);
};

class NPC {
public: //lol. "class"
	vec2f position;
	float heading;
	void update(float dt);
	void draw();
};
