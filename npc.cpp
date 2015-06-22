
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>

#include <pyarlib/includegl.h>
#include <pyarlib/util.h>
#include <pyarlib/vec.h>
#include "npc.h"

using namespace std;

void NPCGroup::init()
{
	frames = 1;
	texture = 0;
	moveSpeed = 1.0f;
	aspectRatio = 1.0f;
	animSpeed = 10.0f;
	gridRes = vec2i(20);
	gridArea.origin = vec2f(0.0f);
	gridArea.size = vec2f(100.0f);
	grid.resize(gridRes.x * gridRes.y);
}

void NPCGroup::update(float dt)
{
	for (auto l : grid)
		l.clear();
	
	for (auto n : all)
	{
		vec2i p = toGridPos(n->position);
		p = vmin(vmax(p, vec2i(0)), gridRes-1);
		grid[p.y*gridRes.x+p.x].push_back(n);
	}	

	for (auto n : all)
	{
		n->time += dt;
		n->moveTimer -= dt;
		if (n->moveTimer < 0.0f)
			doAI(n);
		vec2f dir = (n->movingTo - n->position).unit();
		n->position += dir * dt * moveSpeed;
		n->turningTo = rot2f::fromVec(vec3f(dir.x, 0.0f, dir.y)).y;
		if (n->turningTo - n->heading > pi)
			n->heading += 2.0f * pi;
		if (n->turningTo - n->heading < -pi)
			n->heading -= 2.0f * pi;
		n->heading -= mysign(n->heading - n->turningTo) * mymin(myabs(n->heading - n->turningTo), dt * 20.0f);
	}
	
	
}

void NPCGroup::doAI(NPC* n)
{
	n->moveTimer = UNIT_RAND * 2.0f;
	n->movingTo = n->position + (vec2f(UNIT_RAND, UNIT_RAND) - vec2f(0.5f)).unit()*10.0f;
}

vec2f NPCGroup::toGridPos(vec2f pos)
{
	return (gridRes * (pos - gridArea.origin)) / gridArea.size;
}

float NPCGroup::density(vec2f position, float radius)
{
	vec2i p = toGridPos(position);
	int r = (int)ceil(gridRes.x * radius / gridArea.size.x);
	int c = 0;
	for (int y = mymin(0, p.y-r); y < mymax(gridRes.y-1, p.y+r); ++y)
	{
		for (int x = mymin(0, p.x-r); x < mymax(gridRes.x-1, p.x+r); ++x)
		{
			c += (int)grid[y*gridRes.x+x].size();
		}
	}
	return c;
}

void NPCGroup::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto n : all)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glTranslatef(n->position.x, 0.0f, n->position.y);
		glRotatef(n->heading * 180.0f/pi, 0, 1, 0);
		float f = mymod((int)(n->time * animSpeed), (int)frames) / (float)frames;
		float fd = 1.0f/frames;
		glBegin(GL_QUADS);
		glTexCoord2f(f, 0); glVertex3f(-aspectRatio, 0, -1);
		glTexCoord2f(f+fd, 0); glVertex3f(aspectRatio, 0, -1);
		glTexCoord2f(f+fd, 1); glVertex3f(aspectRatio, 0, 1);
		glTexCoord2f(f, 1); glVertex3f(-aspectRatio, 0, 1);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	glDisable(GL_BLEND);
}

void NPCGroup::onAdd(NPC* n)
{
	all.push_back(n);
	cout << "New NPC " << n->position << endl;
}

void NPCGroup::rem(NPC* npc)
{
	//broken:
	//http://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
	//all.erase(std::remove(all.begin(), all.end(), npc), all.end());
	
	int i = 0;
	for (auto n : all)
	{
		if (npc == n)
			break;
		++i;
	}
	all.erase(all.begin()+i);
}

int NPCGroup::count()
{
	return all.size();
}

NPC::NPC()
{
	position = vec2f(0.0f);
	heading = 0.0f;
	movingTo = vec2f(0.0f);
	moveTimer = 0.0f;
	time = 0.0f;
}
