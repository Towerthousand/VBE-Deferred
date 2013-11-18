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
	ball->pos.y = 2;
	ball->scale = vec3f(3.0f);

	DeferredModel* box = new DeferredModel("box","awesome");
	box->addTo(renderer);
	box->rot.x = 180;
	box->pos.y = -10;
	box->scale = vec3f(10.0f);

	DeferredModel* monkey = new DeferredModel("monkey","null");
	monkey->addTo(renderer);
	monkey->scale = vec3f(7.0f);
	monkey->pos = vec3f(0.0f,8,-10);
	monkey->rot.x = 45;

    DeferredLight* light = new DeferredLight();
    light->addTo(renderer);
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
		vec3f(1,-1,-1),vec3f(1,1,-1),vec3f(-1,1,-1),
		vec3f(-1,1,-1),vec3f(-1,-1,-1),vec3f(1,-1,-1)
	};
	Mesh* quad = Mesh::loadEmpty(Vertex::Format(elems));
	quad->setVertexData(&data[0],6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad",quad);
    Meshes.add("ball",Mesh::loadFromFile("data/meshes/lol.obj"));
    Meshes.add("box",Mesh::loadFromFile("data/meshes/cube.obj"));
	Meshes.add("monkey",Mesh::loadFromFile("data/meshes/monkey.obj"));
    //textures
    Textures.add("lava",Texture::loadFromFile("data/textures/lava.png"));
    Textures.add("awesome",Texture::loadFromFile("data/textures/awesome.png"));
	char pixels[4] = {char(200),char(20),char(20),char(255)};
	Textures.add("null",Texture::loadFromRaw(pixels,1,1));

	//program
	Programs.add("standardDeferred",ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert","data/shaders/standardDeferred.frag"));
	Programs.add("screenQuad",ShaderProgram::loadFromFile("data/shaders/quad.vert","data/shaders/quad.frag"));
    Programs.add("lightQuad",ShaderProgram::loadFromFile("data/shaders/light.vert","data/shaders/light.frag"));
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		VBE_LOG("FPS: " << fpsCount << ". Amount of GameObjects: " << getGame()->getObjectCount());
		debugCounter--;
		fpsCount = 0;
	}
}
