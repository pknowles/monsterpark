
#include <pyarlib/includegl.h>
#include "npc.h"

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
	
	//for (auto n : all)
		

	for (auto n : all)
	{
		n->anim += dt;
		vec3f dir = (n->movingTo - n->position).unit();
		n->position += dir * dt;
		n->heading = rot2f::fromVec(dir).y;
	}
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

NPC::NPC()
{
	position = vec2f(0.0f);
	heading = 0.0f;
	movingTo = vec2f(0.0f);
	anim = 0.0f;
}
