
#include "npc.h"

class PreyGroup;

class PredGroup : public NPCGroup {
public:
	PreyGroup* prey;
	virtual void init();
	virtual void update(float dt);
	virtual void draw();
	virtual NPC* add(vec2f spawn);
};

struct Pred : public NPC {
};
