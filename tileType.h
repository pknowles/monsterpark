#pragma once

#include <pyarlib/pyarlib.h>

#include <vector>

class TileType
{
public:
	int id;
	bool walkable;
	float health;
	vec3f color;

public:
	TileType(int id = 0, bool walkable = false, float health = 0.0f, const vec3f &color = vec3f(0, 0, 0));
};

class TileTypes
{
public:
	static std::vector<TileType> types;

	static void init();
};

