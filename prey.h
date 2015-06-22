
#include "npc.h"

class PredGroup;

struct MoneyIcon {
	vec2f pos;
	int amount;
	float time;
};

class PreyGroup : public NPCGroup {
public:
	PreyGroup();
	std::vector<MoneyIcon> moneyIcons;
	PredGroup* predators;
	int income;
	int collectIncome();
	void newIncome(vec2f pos, int money);
	virtual void init();
	virtual void update(float dt, Grid *tileGrid);
	virtual void draw();
	virtual NPC* add(vec2f spawn);
};

struct Prey : public NPC {
	Prey();
	float moneyTime;
};
