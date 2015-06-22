/* Copyright 2011 Pyarelal Knowles, under GNU LGPL (see LICENCE.txt) */

/*
Visual studio *spit* *spit* *spit* steps:
1. Add entire pyarlib directory to a pyarlib library project or your main executable
2. Add resources.rc to your main executable
3. Include these libraries (project properties->linker->input->additional dependecies):
	glew32.lib;freetype248.lib;SDL2.lib;libpng.lib;zlib.lib;opengl32.lib;glu32.lib
4. Some files (currently just "pyarlib/mesh/simpleobj/obj.c") need to be compiled as C++
	project properties->C++->advanced->compile as
5. Build (this file, template.txt, is an example main.cpp file)
	the following include might need " quotes and a local path instead
*/

#include <pyarlib/pyarlib.h>


#include "grid.h"
#include "gridFly.h"
#include "tileType.h"

#include "preds.h"
#include "prey.h"


Jeltz jeltz("Jeltz");
JeltzFly fly;
GridFly gridFly;
JeltzGUI gui;

Grid grid;

VBOMesh sphere;

QG::Label gameStatus;
QG::DropSelect buildType("Build Type");

PreyGroup prey;
PredGroup preds;

int maxPrey = 10;
vec2f spawnPoint = vec2f(50.0f, 5.0f);

void changeBuildType()
{
	//may get called a few times each time you change it. sorry :(
	printf("%i\n", buildType.selected);
}

void update(float dt)
{
	if (jeltz.buttonDown("`"))
	{
		gui.visible = !gui.visible;
		gui.fps.print = !gui.visible;
	}
	
	gameStatus.textf("Status:\n%f", dt);
	
	prey.update(dt);
	preds.update(dt);
	
	static float reloadTimer = 0.0f;
	reloadTimer -= dt;
	if (reloadTimer < 0.0f)
	{
		reloadTimer = 1.0f;
		if (Shader::reloadModified())
			jeltz.postUnfocusedRedisplay();
	}

	auto mousePos = gridFly.getMousePos(jeltz.mousePosN());
	grid.update(dt, mousePos);
	
	if (buildType.selected != 2)
	{
		// Button pressed, dinosaur not selected
		if (jeltz.button("LButton"))
			grid.startPlacing();
		else
			grid.endPlacing(buildType.selected);
	}
	else
	{
		// Button pressed, dinosaur selected
		if (jeltz.buttonDown("LButton"))
			preds.add(mousePos);
	}
	
	//if we can spawn more at the entrance
	if (prey.count() < maxPrey && prey.density(spawnPoint, 5.0f) < 2)
	{
		prey.add(spawnPoint);
	}
}

void display()
{
	//fly.uploadCamera();
	glDisable(GL_DEPTH_TEST);
	
	gridFly.uploadCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	grid.draw();
	prey.draw();
	preds.draw();
}

int main()
{
	jeltz.setUpdate(update);
	jeltz.setDisplay(display);
	jeltz.add(&gui);
	//jeltz.add(&fly);
	jeltz.add(&gridFly);
	jeltz.init();
	
	gui.controls.add(gameStatus);
	gui.controls.add(buildType);
	buildType.add("Wall");
	buildType.add("Not-Wall");
	buildType.add("Dinosaur");
	buildType.capture(QG::CLICK, changeBuildType);

	Material::defaultAnisotropy = 16;
	VBOMeshOBJ::registerLoader();
	VBOMesh3DS::registerLoader();
	VBOMeshCTM::registerLoader();
	VBOMeshIFS::registerLoader();
	FileFinder::addDir(Config::getString("models"));

	sphere = VBOMesh::grid(vec2i(64, 32), VBOMesh::paramSphere);
	sphere.upload();

	TileTypes::init();
	prey.init();
	preds.init();

	jeltz.run();
	return 0;
}
