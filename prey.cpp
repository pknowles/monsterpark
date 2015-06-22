
#include <pyarlib/pyarlib.h>
#include "prey.h"

void PreyGroup::init()
{
	QI::ImagePNG img;
	img.loadImage("preds.png");
	texture = img.bufferTexture();
}

NPC* PreyGroup::add(vec2f spawn)
{
	Prey* pred = new Prey;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}
