
#include <pyarlib/pyarlib.h>
#include "preds.h"

void PredGroup::init()
{
	QI::ImagePNG img;
	img.loadImage("preds.png");
	texture = img.bufferTexture();
}

NPC* PredGroup::add(vec2f spawn)
{
	Pred* pred = new Pred;
	pred->position = spawn;
	onAdd(pred);
	return pred;
}
