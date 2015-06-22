#pragma once

#include <pyarlib/vec.h>
#include <vector>
#include "util.h"

class NPC;

class NPCGroup {
protected:
	unsigned int texture;
	typedef std::vector<NPC*> NPCList;
	NPCList all;
	std::vector<NPCList> grid;
	vec2i gridRes;
	pyarlib::Box gridArea;
	virtual void onAdd(NPC* n);
public:
	virtual void init();
	virtual void draw();
	virtual void update(float dt);
	virtual NPC* add(vec2f spawn) =0;
	virtual void rem(NPC* npc);
};

struct NPC {
	vec2f position;
	float heading;
	vec2f movingTo;
	float anim;
	NPC();
};
