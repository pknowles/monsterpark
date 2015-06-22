
#include "npc.h"

class PreyGroup : public NPCGroup {
public:
	virtual void init();
	virtual NPC* add(vec2f spawn);
};

class Prey : public NPC {
};
