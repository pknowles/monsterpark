
#include <map>
#include <vector>
#include <string>
#include <set>
#include <assert.h>

#include <pyarlib/pyarlib.h>
#include <pyarlib/text.h>
#include "prey.h"
#include "preds.h"

using namespace std;

map<std::string, Text*> textCache;

Text* getText(std::string text)
{
	if (textCache.find(text) == textCache.end())
	{
		cout << "New Text: " << text << endl;
		textCache[text] = new Text(Config::getString("font"), 42);
		textCache[text]->colour = vec4f(1.0f);
		*textCache[text] = text;
	}
	return textCache[text];
}

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
	moneyIcons.back().time = 2.0;
	moneyIcons.back().amount = money;
}
	
PreyGroup::PreyGroup()
{
	predators = NULL;
	income = 0;
}

void PreyGroup::init()
{
	NPCGroup::init();
	QI::ImagePNG img;
	img.loadImage("prey.png");
	texture = img.bufferTexture();
	frames = 8;
	animSpeed = 10.0f;
	moveSpeed = 2.0;
	aspectRatio = img.width / (float)(img.height * frames);
}

void PreyGroup::update(float dt, Grid *tileGrid)
{
	NPCGroup::update(dt, tileGrid);
	
	if (!predators)
		return;
	
	for (auto& not_n : all)
	{
		Prey* n = (Prey*)not_n;
		n->moneyTime -= dt;
		if (n->moneyTime < 0.0f)
		{
			n->moneyTime += 5.0f;
			int density = predators->density(n->position, 10.0f);
			if (density > 0)
				newIncome(n->position, density);
		}
	}
	
	for (auto t = moneyIcons.begin(); t != moneyIcons.end();)
	{
		if (t->time > 0.0f)
		{
			t->time -= dt;
			t->pos.y += dt * 0.1;
			++t;
		}
		else
		{
			t = moneyIcons.erase(t);
		}
	}
}

void PreyGroup::draw()
{
	NPCGroup::draw();

	mat44 mv, proj;
	glGetFloatv(GL_PROJECTION_MATRIX, proj.m);
	glGetFloatv(GL_MODELVIEW_MATRIX, mv.m);
	mat44 mvp = proj * mv;
	
	vec4i vp;
	glGetIntegerv(GL_VIEWPORT, (int*)&vp);
	mat44 window = mat44::translate(-1.0, -1.0, 0.0) *
			mat44::scale(2.0f/vp.z, 2.0f/vp.w, -0.01);
	
	for (auto& t : moneyIcons)
	{
		vec4f clip = mvp * vec4f(t.pos, 0.0f, 1.0f);
		clip /= clip.w;
		vec2i pixelPos = (clip.xy() * 0.5 + 0.5) * vp.zw();
		getText(intToString(t.amount))->draw(window * mat44::translate(vec3f(pixelPos, 0)));
	}
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




