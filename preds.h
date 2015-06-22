
#include "npc.h"

class PreyGroup;

class PredGroup : public NPCGroup {
public:
	PreyGroup* prey;
	virtual void init();
	virtual NPC* add(vec2f spawn);
};

struct Pred : public NPC {
};
