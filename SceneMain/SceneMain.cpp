#include "SceneMain.hpp"
#include "PlayerCamera.hpp"
#include "DeferredContainer.hpp"
#include "DeferredModel.hpp"

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

	//add ball
	DeferredModel* ball = new DeferredModel("ball","lava");
	ball->addTo(renderer);
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
    //textures
    Textures.add("lava",Texture::loadFromFile("data/textures/lava.png"));
    Textures.add("awesome",Texture::loadFromFile("data/textures/awesome.png"));

	//program
	Programs.add("standardDeferred",ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert","data/shaders/standardDeferred.frag"));
	Programs.add("screenQuad",ShaderProgram::loadFromFile("data/shaders/quad.vert","data/shaders/quad.frag"));
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
