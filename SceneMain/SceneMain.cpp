#include "SceneMain.hpp"
#include "PlayerCamera.hpp"
#include "DeferredContainer.hpp"
#include "DeferredModel.hpp"
#include "DeferredLight.hpp"
#include "particles/LightParticleEmitter.hpp"
#include "particles/ParticleSystem.hpp"

SceneMain::SceneMain() : debugCounter(0.0), fpsCount(0) {
	this->setName("SCENE");

	loadResources();

	//GL stuff..:
	GL_ASSERT(glClearColor(0, 0, 0, 1));
	GL_ASSERT(glEnable(GL_DEPTH_TEST));
	GL_ASSERT(glEnable(GL_BLEND));
	GL_ASSERT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL_ASSERT(glDepthFunc(GL_LEQUAL));
	GL_ASSERT(glEnable(GL_CULL_FACE)); //enable backface culling
	GL_ASSERT(glCullFace(GL_BACK));

	//add player cam
	PlayerCamera* pCam = new PlayerCamera("playerCam", vec3f(0, 10, 15));
	pCam->projection = glm::perspective(60.0f, float(Environment::getScreen()->getWidth())/float(Environment::getScreen()->getHeight()), 0.01f, 100.0f);
	pCam->addTo(this);

	//add sun cam
	Camera* sCam = new Camera("sunCam",vec3f(-10,10,10));
	sCam->lookInDir(vec3f(-1,-1,0));
	sCam->projection = glm::ortho<float>(-25,25,-25,25,-100, 50);
	sCam->addTo(this);

	//add deferred renderer
	DeferredContainer* renderer = new DeferredContainer();
	renderer->addTo(this);

	ParticleSystem* system = new ParticleSystem();
	system->addTo(renderer);
	system->setTextureSheet(Textures2D.get("particleSheet"), 2);

    DeferredModel* ball = new DeferredModel("ball", "lava");
	ball->addTo(renderer);
	ball->pos.y = -1;
	ball->scale = vec3f(3.0f);

	DeferredModel* box = new DeferredModel("box", "awesome");
	box->addTo(renderer);
    box->pos.y = -10;
	box->scale = vec3f(10.0f);

	DeferredModel* monkeyRed = new DeferredModel("monkey", "nullRed");
    monkeyRed->setName("monkeyRed");
	monkeyRed->addTo(renderer);
    monkeyRed->scale = vec3f(5.0f);
    monkeyRed->lookAt = vec3f(0);

	DeferredModel* monkeyGreen = new DeferredModel("monkey", "nullGreen");
    monkeyGreen->setName("monkeyGreen");
	monkeyGreen->addTo(renderer);
    monkeyGreen->lookAt = vec3f(0);
    monkeyGreen->scale = vec3f(5.0f);

	DeferredModel* monkeyBlue = new DeferredModel("monkey", "nullBlue");
    monkeyBlue->setName("monkeyBlue");
	monkeyBlue->addTo(renderer);
    monkeyBlue->lookAt = vec3f(0);
    monkeyBlue->scale = vec3f(5.0f);

    DeferredModel* monkeyWhite = new DeferredModel("monkey", "nullWhite");
    monkeyWhite->setName("monkeyWhite");
	monkeyWhite->addTo(renderer);
    monkeyWhite->lookAt = vec3f(0);
    monkeyWhite->scale = vec3f(5.0f);

	DeferredLight* light1 = new DeferredLight();
	light1->addTo(renderer);
	light1->setName("light1");
	light1->color = vec3f(1, 0, 0);

    ParticleEmitter* emitter1 = new LightParticleEmitter(vec3f(1.0, 0.1, 0.03));
	emitter1->addTo(light1);

	DeferredModel* ballLight1 = new DeferredModel("ball", "nullRed", 1.0, 0.0);
	ballLight1->addTo(light1);
	ballLight1->scale = vec3f(0.5f);

	DeferredLight* light2 = new DeferredLight();
	light2->addTo(renderer);
	light2->setName("light2");
	light2->color = vec3f(0, 1, 0);

	ParticleEmitter* emitter2 = new LightParticleEmitter(vec3f(0.1, 1.0, 0.03));
	emitter2->addTo(light2);

	DeferredModel* ballLight2 = new DeferredModel("ball", "nullGreen", 1.0, 0.0);
	ballLight2->addTo(light2);
	ballLight2->scale = vec3f(0.5f);

	DeferredLight* light3 = new DeferredLight();
	light3->addTo(renderer);
	light3->setName("light3");
	light3->color = vec3f(0, 0, 1);

	ParticleEmitter* emitter3 = new LightParticleEmitter(vec3f(0.03, 0.1, 1.0));
	emitter3->addTo(light3);

	DeferredModel* ballLight3 = new DeferredModel("ball", "nullBlue", 1.0, 0.0);
	ballLight3->addTo(light3);
	ballLight3->scale = vec3f(0.5f);

	DeferredLight* light4 = new DeferredLight();
	light4->addTo(renderer);
	light4->setName("light4");
	light4->color = vec3f(1, 1, 1);

	ParticleEmitter* emitter4 = new LightParticleEmitter(vec3f(0.5, 0.3, 0.6));
	emitter4->addTo(light4);

	DeferredModel* ballLight4 = new DeferredModel("ball", "nullBlack", 1.0, 0.0);
	ballLight4->addTo(light4);
	ballLight4->scale = vec3f(0.5f);
}

SceneMain::~SceneMain() {
	Textures2D.clear();
	Meshes.clear();
	Programs.clear();
}

void SceneMain::loadResources() {
	//meshes
	std::vector<Vertex::Element> elems = {
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3))
	};
	std::vector<vec3f> data = {
		vec3f(1, -1, 0), vec3f(1, 1, 0), vec3f(-1, 1, 0),
		vec3f(-1, 1, 0), vec3f(-1, -1, 0), vec3f(1, -1, 0)
	};
	Mesh* quad = Mesh::loadEmpty(Vertex::Format(elems));
	quad->setVertexData(&data[0], 6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad", quad);
	Meshes.add("ball", Mesh::loadFromFile("data/meshes/sphere.obj"));
	Meshes.add("box", Mesh::loadFromFile("data/meshes/cube.obj"));
	Meshes.add("monkey", Mesh::loadFromFile("data/meshes/monkey.obj"));

	//textures
	Textures2D.add("particleSheet", Texture2D::createFromFile("data/textures/particleSheet.png"));
	Textures2D.add("lava", Texture2D::createFromFile("data/textures/lava.png"));
	Textures2D.add("awesome", Texture2D::createFromFile("data/textures/awesome.png"));
	char pixels[4] = {char(200), char(20), char(20), char(255)};
	Textures2D.add("nullRed", Texture2D::createFromRaw(pixels, 1, 1));
	char pixels2[4] = {char(20), char(200), char(20), char(255)};
	Textures2D.add("nullGreen", Texture2D::createFromRaw(pixels2, 1, 1));
	char pixels3[4] = {char(20), char(20), char(200), char(255)};
	Textures2D.add("nullBlue", Texture2D::createFromRaw(pixels3, 1, 1));
	char pixels4[4] = {char(70), char(30), char(80), char(255)};
	Textures2D.add("nullBlack", Texture2D::createFromRaw(pixels4, 1, 1));
	char pixels5[4] = {char(255), char(255), char(255), char(255)};
	Textures2D.add("nullWhite", Texture2D::createFromRaw(pixels5, 1, 1));

	//program
	Programs.add("deferredModel", ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert", "data/shaders/standardDeferred.frag"));
	Programs.add("deferredLight", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/light.frag"));
	Programs.add("ambientPass", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/ambientPass.frag"));
	Programs.add("blurPassVertical", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurPassVertical.frag"));
	Programs.add("blurPassHoritzontal", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurPassHoritzontal.frag"));
	Programs.add("textureToScreen", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/quad.frag"));
	Programs.add("blurMaskPass", ShaderProgram::loadFromFile("data/shaders/quad.vert", "data/shaders/blurMaskPass.frag"));
	Programs.add("depthShader", ShaderProgram::loadFromFile("data/shaders/depth.vert","data/shaders/depth.frag"));
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
        ParticleSystem* sys = (ParticleSystem*)getGame()->getObjectByName("particleSystem");
		VBE_LOG("FPS: " << fpsCount << " Particles: " << sys->getParticleCount());
		debugCounter--;
		fpsCount = 0;
	}
	if(!Environment::getKeyboard()->isKeyHeld(Keyboard::Space)) {
		float circleWidth = 5+3*sin(Environment::getClock());
		DeferredLight* light1 = (DeferredLight*)getGame()->getObjectByName("light1");
		light1->pos = vec3f(circleWidth*sin(5*Environment::getClock()), 2, circleWidth*cos(5*Environment::getClock()));
		DeferredLight* light2 = (DeferredLight*)getGame()->getObjectByName("light2");
		light2->pos = vec3f(circleWidth*sin(5*(Environment::getClock()-M_PI)), 2, circleWidth*cos(5*(Environment::getClock()-M_PI)));
		DeferredLight* light3 = (DeferredLight*)getGame()->getObjectByName("light3");
		light3->pos = vec3f(circleWidth*sin(5*(Environment::getClock()-M_PI/2)), 2, circleWidth*cos(5*(Environment::getClock()-M_PI/2)));
		DeferredLight* light4 = (DeferredLight*)getGame()->getObjectByName("light4");
		light4->pos = vec3f(circleWidth*sin(5*(Environment::getClock()-M_PI*1.5)), 2, circleWidth*cos(5*(Environment::getClock()-M_PI*1.5)));
	}
	if(!Environment::getKeyboard()->isKeyHeld(Keyboard::Space)) {
        float circleWidth = 10;
        int speed = 1;
        DeferredModel* model1 = (DeferredModel*)getGame()->getObjectByName("monkeyRed");
		model1->pos = vec3f(circleWidth*sin(speed*Environment::getClock()), 8, circleWidth*cos(speed*Environment::getClock()));
        DeferredModel* model2 = (DeferredModel*)getGame()->getObjectByName("monkeyGreen");
		model2->pos = vec3f(circleWidth*sin(speed*(Environment::getClock()-M_PI)), 8, circleWidth*cos(speed*(Environment::getClock()-M_PI)));
        DeferredModel* model3 = (DeferredModel*)getGame()->getObjectByName("monkeyBlue");
		model3->pos = vec3f(circleWidth*sin(speed*(Environment::getClock()-M_PI/2)), 8, circleWidth*cos(speed*(Environment::getClock()-M_PI/2)));
        DeferredModel* model4 = (DeferredModel*)getGame()->getObjectByName("monkeyWhite");
		model4->pos = vec3f(circleWidth*sin(speed*(Environment::getClock()-M_PI*1.5)), 8, circleWidth*cos(speed*(Environment::getClock()-M_PI*1.5)));
    }
	if(Environment::getMouse()->isButtonPressed(Mouse::Left)) {
		Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
		DeferredContainer* renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
		vec3f color = glm::abs(glm::sphericalRand(1.0f));
		DeferredLight* l = new DeferredLight();
		l->addTo(renderer);
		l->vel = cam->getForward()*10.0f;
		l->pos = cam->pos;
		l->color = color;
		l->radius = 20;

		ParticleEmitter* e = new LightParticleEmitter(color);
		e->addTo(l);

		DeferredModel* m = new DeferredModel("ball", "nullWhite", 1.0, 0.0);
		m->addTo(l);
		m->scale = vec3f(0.5f);
	}
}
