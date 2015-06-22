
#include "npc.h"

class PreyGroup;

class PredGroup : public NPCGroup {
public:
	PreyGroup* prey;
	virtual void init();
	virtual void update(float dt, Grid *tileGrid);
	virtual void draw();
	virtual NPC* add(vec2f spawn);
	virtual void doAICollision(float dt, NPC *npc, Tile *tile) override;
};

struct Pred : public NPC {
};
