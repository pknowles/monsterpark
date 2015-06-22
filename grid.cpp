#include "grid.h"

#include <GL/gl.h>

vec2i Grid::getTilePos()
{
	return vec2i((int) (mousePos.x / (float) cols * size.x), (int) (mousePos.z / (float) rows * size.y));
}

void Grid::drawLines()
{
	// TODO: not do this in immediate mode

	glBegin(GL_LINES);
	for (unsigned int x = 0; x <= cols; x++)
	{
		float xPos = (x / (float) cols) * size.x;

		glVertex3f(xPos, 0, 0);
		glVertex3f(xPos, 0, size.y);
	}
	
	for (unsigned int y = 0; y <= rows; y++)
	{
		float yPos = (y / (float) rows) * size.y;

		glVertex3f(0, 0, yPos);
		glVertex3f(size.x, 0, yPos);
	}
	glEnd();
}

void Grid::drawTileQuad(const vec2i &pos, const vec3f &color)
{
	vec2f cellSize(size.x / (float) cols, size.y / (float) rows);

	glColor3f(color.x, color.y, color.z);

	glBegin(GL_QUADS);
	glVertex3f(pos.x,              0, pos.y);
	glVertex3f(pos.x,              0, pos.y + cellSize.y);
	glVertex3f(pos.x + cellSize.x, 0, pos.y + cellSize.y);
	glVertex3f(pos.x + cellSize.x, 0, pos.y);
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
				drawTileQuad(vec2i(i, j), vec3f(1, 0, 0));
		}
	}
}





Grid::Grid(float width, float height, unsigned int rows, unsigned int cols) : rows(rows), cols(cols)
{
	size = vec2f(width, height);
	mousePos = vec3f(0, 0, 0);
	
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

void Grid::update(float dt, const vec3f &mousePos)
{
	this->mousePos = mousePos;

	this->mousePos.x = myclamp(this->mousePos.x, 0.0f, size.x - (size.x / (float) cols / 2.0f));
	this->mousePos.z = myclamp(this->mousePos.z, 0.0f, size.y - (size.y / (float) rows / 2.0f));
}

void Grid::draw()
{
	bool editMode = true;
	
	if (editMode)
	{
		drawLines();
		drawTiles();
		drawTileQuad(getTilePos(), vec3f(0, 0, 1));
	}
}

void Grid::placeTile()
{
	vec2i mouse = getTilePos();

	if (tiles[mouse.x][mouse.y].id != -1)
		return;

	tiles[mouse.x][mouse.y].id = 1;
}

