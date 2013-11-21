#include "SceneMain.hpp"
#include "PlayerCamera.hpp"
#include "DeferredContainer.hpp"
#include "DeferredModel.hpp"
#include "DeferredLight.hpp"

SceneMain::SceneMain() : debugCounter(0.0), fpsCount(0) {
	this->setName("SCENE");

	loadResources();

	//GL stuff..:
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE); //enable backface culling
	glCullFace(GL_BACK);

	//add player cam
	PlayerCamera* pCam = new PlayerCamera("playerCam",vec3f(0,10,15),vec3f(45,0,0));
	pCam->projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);
	pCam->addTo(this);

	//add deferred renderer
	DeferredContainer*renderer = new DeferredContainer();
	renderer->addTo(this);

	DeferredModel* ball = new DeferredModel("ball","lava");
	ball->addTo(renderer);
	ball->pos.y = -1;
	ball->scale = vec3f(3.0f);

	DeferredModel* box = new DeferredModel("box","awesome");
	box->addTo(renderer);
	box->rot.x = 180;
	box->pos.y = -10;
	box->scale = vec3f(10.0f);

	DeferredModel* monkeyRed = new DeferredModel("monkey","nullRed");
	monkeyRed->addTo(renderer);
	monkeyRed->scale = vec3f(7.0f);
	monkeyRed->pos = vec3f(0.0f,8,-10);
	monkeyRed->rot.x = 45;

	DeferredModel* monkeyGreen = new DeferredModel("monkey","nullGreen");
	monkeyGreen->addTo(renderer);
	monkeyGreen->scale = vec3f(7.0f);
	monkeyGreen->pos = vec3f(0.0f,8,10);
	monkeyGreen->rot.x = 45;
	monkeyGreen->rot.y = 180;

	DeferredModel* monkeyBlue = new DeferredModel("monkey","nullBlue");
	monkeyBlue->addTo(renderer);
	monkeyBlue->scale = vec3f(7.0f);
	monkeyBlue->pos = vec3f(-10,8,0);
	monkeyBlue->rot.x = 45;
	monkeyBlue->rot.y = 90;

	DeferredModel* monkeyWhite = new DeferredModel("monkey","nullWhite");
	monkeyWhite->addTo(renderer);
	monkeyWhite->scale = vec3f(7.0f);
	monkeyWhite->pos = vec3f(10,8,0);
	monkeyWhite->rot.x = 45;
	monkeyWhite->rot.y = -90;

	DeferredLight* light = new DeferredLight();
	light->addTo(renderer);
	light->setName("light");
	light->color = vec3f(1,0,0);

	DeferredModel* ballLight = new DeferredModel("ball","nullRed");
	ballLight->addTo(renderer);
	ballLight->scale = vec3f(0.5f);
	ballLight->setName("ballLight");

	DeferredLight* light2 = new DeferredLight();
	light2->addTo(renderer);
	light2->setName("light2");
	light2->color = vec3f(0,1,0);

	DeferredModel* ballLight2 = new DeferredModel("ball","nullGreen");
	ballLight2->addTo(renderer);
	ballLight2->scale = vec3f(0.5f);
	ballLight2->setName("ballLight2");

	DeferredLight* light3 = new DeferredLight();
	light3->addTo(renderer);
	light3->setName("light3");
	light3->color = vec3f(0,0,1);

	DeferredModel* ballLight3 = new DeferredModel("ball","nullBlue");
	ballLight3->addTo(renderer);
	ballLight3->scale = vec3f(0.5f);
	ballLight3->setName("ballLight3");

	DeferredLight* light4 = new DeferredLight();
	light4->addTo(renderer);
	light4->setName("light4");
	light4->color = vec3f(1,1,1);

	DeferredModel* ballLight4 = new DeferredModel("ball","nullWhite");
	ballLight4->addTo(renderer);
	ballLight4->scale = vec3f(0.5f);
	ballLight4->setName("ballLight4");
}

SceneMain::~SceneMain() {
	Textures.clear();
	Meshes.clear();
	Programs.clear();
	AudioManager::clear();
}

void SceneMain::loadResources() {
	//meshes
	std::vector<Vertex::Element> elems = {
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position,Vertex::Element::Float,3))
	};
	std::vector<vec3f> data = {
        vec3f(1,-1,0),vec3f(1,1,0),vec3f(-1,1,0),
        vec3f(-1,1,0),vec3f(-1,-1,0),vec3f(1,-1,0)
	};
	Mesh* quad = Mesh::loadEmpty(Vertex::Format(elems));
	quad->setVertexData(&data[0],6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad",quad);
	Meshes.add("ball",Mesh::loadFromFile("data/meshes/sphere.obj"));
	Meshes.add("box",Mesh::loadFromFile("data/meshes/cube.obj"));
	Meshes.add("monkey",Mesh::loadFromFile("data/meshes/monkey.obj"));
	//textures
	Textures.add("lava",Texture::loadFromFile("data/textures/lava.png"));
	Textures.add("awesome",Texture::loadFromFile("data/textures/awesome.png"));
	char pixels[4] = {char(200),char(20),char(20),char(255)};
	Textures.add("nullRed",Texture::loadFromRaw(pixels,1,1));
	char pixels2[4] = {char(20),char(200),char(20),char(255)};
	Textures.add("nullGreen",Texture::loadFromRaw(pixels2,1,1));
	char pixels3[4] = {char(20),char(20),char(200),char(255)};
	Textures.add("nullBlue",Texture::loadFromRaw(pixels3,1,1));
	char pixels4[4] = {char(255),char(255),char(255),char(255)};
	Textures.add("nullWhite",Texture::loadFromRaw(pixels4,1,1));

	//program
	Programs.add("standardDeferred",ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert","data/shaders/standardDeferred.frag"));
	Programs.add("screenQuad",ShaderProgram::loadFromFile("data/shaders/quad.vert","data/shaders/quad.frag"));
	Programs.add("lightQuad",ShaderProgram::loadFromFile("data/shaders/quad.vert","data/shaders/light.frag"));
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		VBE_LOG("FPS: " << fpsCount << ". Amount of GameObjects: " << getGame()->getObjectCount());
		debugCounter--;
		fpsCount = 0;
	}
	if(!Input::isKeyDown(sf::Keyboard::Space)) {
		float circleWidth = 5+3*sin(GLOBALCLOCK.getElapsedTime().asSeconds());
		DeferredLight* light = (DeferredLight*)getGame()->getObjectByName("light");
		light->pos = vec3f(circleWidth*sin(5*GLOBALCLOCK.getElapsedTime().asSeconds()),2,circleWidth*cos(5*GLOBALCLOCK.getElapsedTime().asSeconds()));
		DeferredModel* ballLight = (DeferredModel*)getGame()->getObjectByName("ballLight");
		ballLight->pos = light->pos;
		DeferredLight* light2 = (DeferredLight*)getGame()->getObjectByName("light2");
		light2->pos = vec3f(circleWidth*sin(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI)),2,circleWidth*cos(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI)));
		DeferredModel* ballLight2 = (DeferredModel*)getGame()->getObjectByName("ballLight2");
		ballLight2->pos = light2->pos;
		DeferredLight* light3 = (DeferredLight*)getGame()->getObjectByName("light3");
		light3->pos = vec3f(circleWidth*sin(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI/2)),2,circleWidth*cos(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI/2)));
		DeferredModel* ballLight3 = (DeferredModel*)getGame()->getObjectByName("ballLight3");
		ballLight3->pos = light3->pos;
		DeferredLight* light4 = (DeferredLight*)getGame()->getObjectByName("light4");
		light4->pos = vec3f(circleWidth*sin(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI*1.5)),2,circleWidth*cos(5*(GLOBALCLOCK.getElapsedTime().asSeconds()-M_PI*1.5)));
		DeferredModel* ballLight4 = (DeferredModel*)getGame()->getObjectByName("ballLight4");
		ballLight4->pos = light4->pos;
	}
}
