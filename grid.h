#pragma once

#include <pyarlib/pyarlib.h>

#include <vector>

struct Tile
{
	int id;
	bool walkable;
	float health;
};

class Grid
{
private:
	vec2f size;

	unsigned int rows;
	unsigned int cols;

	Tile **tiles;

	vec3f mousePos;

	vec2i startPlacePos;
	vec2i endPlacePos;

private:
	vec2i getTilePos();

	void drawLines();
	void drawTileQuad(const vec2i &pos, const vec3f &color);

	void drawTiles();

public:
	Grid(float width = 100, float height = 100, unsigned int rows = 100, unsigned int cols = 100);
	~Grid();

	void update(float dt, const vec3f &mousePos);
	void draw();

	void placeTile();

	void startPlacing();
	void endPlacing();
};

