#pragma once

#include <pyarlib/vec.h>
#include <vector>
#include "util.h"

class NPC;
class Grid;

class NPCGroup {
protected:
	int frames;
	float moveSpeed;
	float animSpeed;
	float aspectRatio;
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
	virtual void update(float dt, Grid *tileGrid);
	virtual NPC* add(vec2f spawn) =0;
	virtual void rem(NPC* npc);
	virtual void doAI(NPC* npc);
	virtual void doAICollision(NPC *npc) =0;
	vec2f toGridPos(vec2f pos);
	float density(vec2f position, float radius);
	int count();
};

struct NPC {
	vec2f position;
	float heading;
	float turningTo;
	vec2f movingTo;
	float moveTimer;
	float time;
	NPC();
};
