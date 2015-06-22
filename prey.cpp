
#include <map>
#include <vector>
#include <string>
#include <set>

#include <pyarlib/pyarlib.h>
#include "prey.h"
#include "preds.h"

int PreyGroup::collectIncome()
{
	int t = income;
	income = 0;
	return t;
}

void PreyGroup::newIncome(vec2f pos, int money)
{
	income += money;
	moneyIcons.push_back(MoneyIcon());
	moneyIcons.back().pos = pos;
	moneyIcons.back().time = 1.0;
}
	
void PreyGroup::init()
{
	predators = NULL;
	income = 0;
	NPCGroup::init();
	QI::ImagePNG img;
	img.loadImage("prey.png");
	texture = img.bufferTexture();
	frames = 8;
	animSpeed = 10.0f;
	moveSpeed = 2.0;
	aspectRatio = img.width / (float)(img.height * frames);
}

void PreyGroup::update(float dt)
{
	NPCGroup::update(dt);
	
	if (!predators)
		return;
	
	for (auto not_n : all)
	{
		Prey* n = (Prey*)not_n;
		n->moneyTime -= dt;
		if (n->moneyTime < 0.0f)
		{
			n->moneyTime += 1.0f;
			int density = predators->density(n->position, 5.0f);
			newIncome(n->position, density);
		}
	}
}

void PreyGroup::draw()
{
	NPCGroup::draw();
	
}

NPC* PreyGroup::add(vec2f spawn)
{
	Prey* pred = new Prey;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}

Prey::Prey()
{
	moneyTime = 1.0f + UNIT_RAND;
}




