
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>

#include <pyarlib/pyarlib.h>
#include "preds.h"

#include "grid.h"

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

void PredGroup::doAICollision(float dt, NPC *npc, Tile *tile)
{
	const float damage = 2.0f;

	tile->health -= damage *dt;
	if (tile->health <= 0)
	{
		tile->color = vec3f(0, 1, 0);
		tile->walkable = true;
	}
}

