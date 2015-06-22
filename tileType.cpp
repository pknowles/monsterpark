#include "tileType.h"

std::vector<TileType> TileTypes::types;

TileType::TileType(int id, bool walkable, float health, const vec3f &color) : id(id), walkable(walkable), health(health), color(color)
{
}



void TileTypes::init()
{
	int nTypes = 2;

	types.resize(nTypes);

	types[0] = {0, false, 10.0f, vec3f(1, 0, 0)};
	types[1] = {1, true, 5.0f, vec3f(0, 1, 0)};
}

