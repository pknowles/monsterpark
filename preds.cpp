
#include <map>
#include <vector>
#include <string>
#include <set>

#include <pyarlib/pyarlib.h>
#include "preds.h"

void PredGroup::init()
{
	NPCGroup::init();
	QI::ImagePNG img;
	img.loadImage("preds.png");
	texture = img.bufferTexture();
	frames = 2;
	animSpeed = 2.0f;
	moveSpeed = 4.0;
	aspectRatio = img.width / (float)(img.height * frames);
}

NPC* PredGroup::add(vec2f spawn)
{
	Pred* pred = new Pred;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}
