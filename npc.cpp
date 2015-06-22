
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <assert.h>

#include <pyarlib/includegl.h>
#include <pyarlib/util.h>
#include <pyarlib/vec.h>
#include "npc.h"

#include "grid.h"

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

void NPCGroup::update(float dt, Grid *tileGrid)
{
	for (auto n = all.begin(); n != all.end();)
	{
		if (toRemove.find(*n) != toRemove.end())
		{
			delete *n;
			n = all.erase(n);
		}
		else
			++n;
	}
	toRemove.clear();

	for (auto& l : grid)
		l.clear();
	
	for (auto& n : all)
	{
		vec2i p = toGridPos(n->position);
		p = vmin(vmax(p, vec2i(0)), gridRes-1);
		grid[p.y*gridRes.x+p.x].push_back(n);
	}	

	for (auto& n : all)
	{
		n->time += dt;
		n->moveTimer -= dt;
		if (n->moveTimer < 0.0f)
			doAI(n);

		vec2f dir = n->movingTo - n->position;
		if (dir.size() > 1.0f)
			dir /= dir.size();

		auto vel = dir * dt * moveSpeed;
		auto newPos = n->position + vel;
		newPos.x = myclamp(newPos.x, 0.0f, tileGrid->getSize().x - 0.1f);
		newPos.y = myclamp(newPos.y, 0.0f, tileGrid->getSize().y - 0.1f);
		
		vec2i tilePos((int) newPos.x, (int) newPos.y);
		auto &tile = tileGrid->getTile(tilePos.x, tilePos.y);

		if (tile.id == -1 || tile.walkable)
			n->position = newPos;
		else
			doAICollision(dt, n, &tile);

		n->turningTo = -rot2f::fromVec(vec3f(dir.x, 0.0f, dir.y)).y;
		if (n->turningTo - n->heading > pi)
			n->heading += 2.0f * pi;
		if (n->turningTo - n->heading < -pi)
			n->heading -= 2.0f * pi;
		n->heading -= mysign(n->heading - n->turningTo) * mymin(myabs(n->heading - n->turningTo), dt * 20.0f);
	}
	
	
}

void NPCGroup::takeDamage(NPC* npc, float damage)
{
	npc->health -= damage;
	if (npc->health < 0.0f)
		rem(npc);
}

void NPCGroup::doAI(NPC* n)
{
	n->moveTimer = UNIT_RAND * 2.0f;
	n->movingTo = n->position + (vec2f(UNIT_RAND, UNIT_RAND) - vec2f(0.5f)) * 8.0f;
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
	for (int y = mymax(0, p.y-r); y < mymin(gridRes.y-1, p.y+r); ++y)
	{
		for (int x = mymax(0, p.x-r); x < mymin(gridRes.x-1, p.x+r); ++x)
		{
			/*if (y*gridRes.x+x < 0 || y*gridRes.x+x >= grid.size())
			{
				cout << position << x << " " << y << endl;
				return 0.0f;
			}*/
			c += (int)grid[y*gridRes.x+x].size();
		}
	}
	return c;
}

NPC* NPCGroup::nearest(vec2f position, float radius)
{
	vec2i p = toGridPos(position);
	int r = (int)ceil(gridRes.x * radius / gridArea.size.x);
	float minDist = radius + 1.0f;
	NPC* n = NULL;
	for (int y = mymax(0, p.y-r); y < mymin(gridRes.y-1, p.y+r); ++y)
	{
		for (int x = mymax(0, p.x-r); x < mymin(gridRes.x-1, p.x+r); ++x)
		{
			for (auto& o : grid[y*gridRes.x+x])
			{
				if (!n || (n->position - o->position).size() < minDist)
					n = o;
			}
		}
	}
	return n;
}

void NPCGroup::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto& n : all)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glTranslatef(n->position.x, n->position.y, 0.0f);
		glRotatef(n->heading * 180.0f/pi, 0, 0, 1);
		float f = mymod((int)(n->time * animSpeed), (int)frames) / (float)frames;
		float fd = 1.0f/frames;
		glBegin(GL_QUADS);
		glTexCoord2f(f, 0); glVertex3f(-aspectRatio, -1, 0);
		glTexCoord2f(f+fd, 0); glVertex3f(aspectRatio, -1, 0);
		glTexCoord2f(f+fd, 1); glVertex3f(aspectRatio, 1, 0);
		glTexCoord2f(f, 1); glVertex3f(-aspectRatio, 1, 0);
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
	toRemove.insert(npc);
}

int NPCGroup::count()
{
	return all.size();
}

NPC::NPC()
{
	health = 100.0f;
	position = vec2f(0.0f);
	heading = 0.0f;
	movingTo = vec2f(0.0f);
	moveTimer = 0.0f;
	time = 0.0f;
}
