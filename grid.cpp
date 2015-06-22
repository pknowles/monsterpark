#include "grid.h"
#include "tileType.h"

#include <pyarlib/includegl.h>

vec2i Grid::getTilePos()
{
	return vec2i((int) (mousePos.x / (float) cols * size.x), (int) (mousePos.y / (float) rows * size.y));
}

void Grid::drawLines()
{
	// TODO: not do this in immediate mode

	glBegin(GL_LINES);
	for (unsigned int x = 0; x <= cols; x++)
	{
		float xPos = (x / (float) cols) * size.x;

		glVertex3f(xPos, 0, 0);
		glVertex3f(xPos, size.y, 0);
	}
	
	for (unsigned int y = 0; y <= rows; y++)
	{
		float yPos = (y / (float) rows) * size.y;

		glVertex3f(0, yPos, 0);
		glVertex3f(size.x, yPos, 0);
	}
	glEnd();
}

void Grid::drawTileQuad(const vec2i &pos, const vec3f &color)
{
	vec2f cellSize(size.x / (float) cols, size.y / (float) rows);

	glColor3f(color.x, color.y, color.z);

	glBegin(GL_QUADS);
	glVertex3f(pos.x,              pos.y, 0);
	glVertex3f(pos.x,              pos.y + cellSize.y, 0);
	glVertex3f(pos.x + cellSize.x, pos.y + cellSize.y, 0);
	glVertex3f(pos.x + cellSize.x, pos.y, 0);
	glEnd();

	glColor3f(1, 1, 1);
}

void Grid::drawPlacement(const vec2i &start, const vec2i &end, const vec3f &color)
{
	vec2f cellSize(size.x / (float) cols, size.y / (float) rows);

	vec2i min = vmin(start, end);
	vec2i max = vmax(start, end) + cellSize;

	glColor3f(color.x, color.y, color.z);

	glBegin(GL_QUADS);

	glVertex3f(min.x, min.y, 0);
	glVertex3f(min.x, max.y, 0);
	glVertex3f(max.x, max.y, 0);
	glVertex3f(max.x, min.y, 0);

	glEnd();

	glColor3f(1, 1, 1);
}

void Grid::drawTiles()
{
	for (unsigned int i = 0; i < cols; i++)
	{
		for (unsigned int j = 0; j < rows; j++)
		{
			if (tiles[i][j].id != -1)
				drawTileQuad(vec2i(i, j), tiles[i][j].color);
		}
	}
}

void Grid::placeTile(unsigned int x, unsigned int y, int type)
{
	if (tiles[x][y].id != -1)
		return;

	auto &tileType = TileTypes::types[type];

	tiles[x][y] = {tileType.id, tileType.walkable, tileType.health, tileType.color};
}





Grid::Grid(float width, float height, unsigned int rows, unsigned int cols) : rows(rows), cols(cols), placing(false)
{
	size = vec2f(width, height);
	mousePos = vec2f(0, 0);
	
	tiles = new Tile*[cols];
	for (unsigned int i = 0; i < cols; i++)
	{
		tiles[i] = new Tile[rows];
		for (unsigned int j = 0; j < rows; j++)
			tiles[i][j].id = -1;
	}
}

Grid::~Grid()
{
	for (unsigned int i = 0; i < cols; i++)
		delete [] tiles[i];
	delete [] tiles;
}

void Grid::update(float dt, const vec2f &mousePos)
{
	this->mousePos = mousePos;

	this->mousePos.x = myclamp(this->mousePos.x, 0.0f, size.x - (size.x / (float) cols / 2.0f));
	this->mousePos.y = myclamp(this->mousePos.y, 0.0f, size.y - (size.y / (float) rows / 2.0f));
}

void Grid::draw()
{
	bool editMode = true;
	
	if (editMode)
	{
		drawLines();
		drawTiles();
		drawTileQuad(getTilePos(), vec3f(0, 0, 1));

		if (placing)
			drawPlacement(startPlacePos, endPlacePos, vec3f(0, 0, 1));
	}
}

void Grid::startPlacing()
{
	if (!placing)
		startPlacePos = getTilePos();
	endPlacePos = getTilePos();

	placing = true;
}

void Grid::endPlacing(int type)
{
	if (!placing)
		return;

	endPlacePos = getTilePos();
	placing = false;

	unsigned int minX = mymin(startPlacePos.x, endPlacePos.x);
	unsigned int maxX = mymax(startPlacePos.x, endPlacePos.x);
	unsigned int minY = mymin(startPlacePos.y, endPlacePos.y);
	unsigned int maxY = mymax(startPlacePos.y, endPlacePos.y);

	for (unsigned int x = minX; x <= maxX; x++)
	{
		for (unsigned int y = minY; y <= maxY; y++)
		{
			placeTile(x, y, type);
		}
	}
}

Tile &Grid::getTile(unsigned int x, unsigned int y)
{
	return tiles[x][y];
}

