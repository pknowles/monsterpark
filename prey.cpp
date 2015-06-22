
#include <map>
#include <vector>
#include <string>
#include <set>

#include <pyarlib/pyarlib.h>
#include "prey.h"

void PreyGroup::init()
{
	NPCGroup::init();
	QI::ImagePNG img;
	img.loadImage("prey.png");
	texture = img.bufferTexture();
	frames = 8;
}

NPC* PreyGroup::add(vec2f spawn)
{
	Prey* pred = new Prey;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}
