
#include "npc.h"

class PredGroup : public NPCGroup {
public:
	virtual void init();
	virtual NPC* add(vec2f spawn);
};

class Pred : public NPC {
};
