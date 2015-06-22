
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
	texture = 0;
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
		n->anim += dt;
		vec3f dir = (n->movingTo - n->position).unit();
		n->position += dir * dt;
		n->heading = rot2f::fromVec(dir).y;
	}
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
	for (auto n : all)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glPushMatrix();
		glTranslatef(n->position.x, 0.0f, n->position.y);
		glRotatef(n->heading, 0, 1, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-1, 0, -1);
		glTexCoord2f(1, 0); glVertex3f(1, 0, -1);
		glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
		glTexCoord2f(0, 1); glVertex3f(-1, 0, 1);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
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
	anim = 0.0f;
}
