

void NPCGroup::update(float dt)
{
	for (auto n : all)
		n->update(dt);
}

void NPCGroup::draw()
{
	for (auto n : all)
		n->draw();
}

NPC* NPCGroup::add(vec2f spawn)
{
}

void NPC::update(float dt)
{
}

void draw()
{
}
