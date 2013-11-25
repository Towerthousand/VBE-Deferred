#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredContainer::DeferredContainer() : gBuffer(NULL), noBlur(NULL), horitzontalBlurred(NULL), drawMode(Deferred) {
	setName("deferred");
	gBuffer = new RenderTarget(SCRWIDTH, SCRHEIGHT);
	gBuffer->addTexture(RenderTarget::DEPTH, Texture::DEPTH_COMPONENT32); //Z-BUFFER
	gBuffer->addTexture(RenderTarget::COLOR0, Texture::RGB8); //COLOR
	gBuffer->addTexture(RenderTarget::COLOR1, Texture::RGBA16F); //NORMAL + brightness
	gBuffer->build();
	gBuffer->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);
	gBuffer->getTextureForAttachment(RenderTarget::COLOR1)->setFilter(GL_NEAREST, GL_NEAREST);

	noBlur = new RenderTarget(SCRWIDTH, SCRHEIGHT);
	noBlur->addRenderBuffer(RenderTarget::DEPTH, Texture::DEPTH_COMPONENT32); //Z-BUFFER
	noBlur->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
	noBlur->build();
	noBlur->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);

	horitzontalBlurred = new RenderTarget(SCRWIDTH/2, SCRHEIGHT/2);
	horitzontalBlurred->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
	horitzontalBlurred->build();
	horitzontalBlurred->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);

	blurred = new RenderTarget(SCRWIDTH/2, SCRHEIGHT/2);
	blurred->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
	blurred->build();
	blurred->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);

	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("ambientPass");
}

DeferredContainer::~
DeferredContainer() {
}

void DeferredContainer::update(float deltaTime) {
    ContainerObject::update(deltaTime);
}

void DeferredContainer::draw() const {
	//G BUFFER
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	drawMode = Deferred;
	RenderTarget::bind(gBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ContainerObject::draw();
	glEnable(GL_BLEND);

	RenderTarget::bind(noBlur);
	glClear(GL_COLOR_BUFFER_BIT);

    //LIGHTS
    drawMode = Light;
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

	ContainerObject::draw();

	quad.program = Programs.get("ambientPass");
	quad.program->uniform("MVP")->set(mat4f(1.0f));
	quad.program->uniform("color0")->set(getColor0());
	quad.program->uniform("color1")->set(getColor1());
	quad.program->uniform("invResolution")->set(vec2f(1.0f/SCRWIDTH, 1.0f/SCRHEIGHT));
	quad.draw();

	RenderTarget::bind(horitzontalBlurred); //draw to screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	quad.program = Programs.get("blurPassHoritzontal");
	quad.program->uniform("MVP")->set(mat4f(1.0f));
	quad.program->uniform("RTScene")->set(noBlur->getTextureForAttachment(RenderTarget::COLOR0));
	quad.program->uniform("invResolution")->set(vec2f(1.0f/(SCRWIDTH/2), 1.0f/(SCRHEIGHT/2)));
	quad.draw();

	RenderTarget::bind(blurred);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	quad.program = Programs.get("blurPassVertical");
	quad.program->uniform("MVP")->set(mat4f(1.0f));
	quad.program->uniform("RTBlurH")->set(horitzontalBlurred->getTextureForAttachment(RenderTarget::COLOR0));
	quad.program->uniform("invResolution")->set(vec2f(1.0f/(SCRWIDTH/2), 1.0f/(SCRHEIGHT/2)));
	quad.draw();

	RenderTarget::bind(nullptr);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	quad.program = Programs.get("textureToScreen");
	quad.program->uniform("MVP")->set(mat4f(1.0f));
	quad.program->uniform("tex")->set(blurred->getTextureForAttachment(RenderTarget::COLOR0));
	quad.program->uniform("invResolution")->set(vec2f(1.0f/(SCRWIDTH), 1.0f/(SCRHEIGHT)));
	quad.draw();

	glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
}

DeferredContainer::DrawMode DeferredContainer::getMode() const {
    return drawMode;
}

Texture *DeferredContainer::getColor0() const {
    return gBuffer->getTextureForAttachment(RenderTarget::COLOR0);
}

Texture *DeferredContainer::getColor1() const {
	return gBuffer->getTextureForAttachment(RenderTarget::COLOR1);
}

Texture *DeferredContainer::getDepth() const {
    return gBuffer->getTextureForAttachment(RenderTarget::DEPTH);
}

