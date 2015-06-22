
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>

#include <pyarlib/pyarlib.h>
#include "prey.h"
#include "preds.h"

using namespace std;

void PredGroup::init()
{
	NPCGroup::init();
	QI::ImagePNG img;
	img.loadImage("pred.png");
	texture = img.bufferTexture();
	frames = 2;
	animSpeed = 2.0f;
	moveSpeed = 4.0;
	aspectRatio = img.width / (float)(img.height * frames);
}

void PredGroup::update(float dt, Grid *tileGrid)
{
	NPCGroup::update(dt, tileGrid);
	
	if (!prey)
		return;
	
	for (auto& not_n : all)
	{
		Pred* n = (Pred*)not_n;
		
		//cout << n->position << endl;
	}
}

void PredGroup::doAI(NPC* npc)
{
	NPCGroup::doAI(npc);

	if (!prey)
		return;
		
	NPC* nearest = prey->nearest(npc->position, 5.0);
	if (nearest)
	{
		float dist = (npc->position - nearest->position).size();
		if (dist < 1.0f)
		{
			prey->takeDamage(nearest, 60.0f);
		}
		npc->movingTo = nearest->position;
	}
}

void PredGroup::draw()
{
	NPCGroup::draw();
}

NPC* PredGroup::add(vec2f spawn)
{
	Pred* pred = new Pred;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}
