#include "DeferredContainer.hpp"

DeferredContainer::DeferredContainer() : gBuffer(NULL), drawMode(Deferred), texOption(1) {
	setName("deferred");
	gBuffer = new RenderTarget(SCRWIDTH,SCRHEIGHT);
	gBuffer->addTexture(RenderTarget::DEPTH,Texture::DEPTH_COMPONENT32); //Z-BUFFER
	gBuffer->addTexture(RenderTarget::COLOR0,Texture::RGBA8); //COLOR
	gBuffer->addTexture(RenderTarget::COLOR1,Texture::RGB16F); //NORMAL
	gBuffer->build();
	gBuffer->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST,GL_NEAREST);
	gBuffer->getTextureForAttachment(RenderTarget::COLOR1)->setFilter(GL_NEAREST,GL_NEAREST);

	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("screenQuad");
}

DeferredContainer::~DeferredContainer() {
}

void DeferredContainer::update(float deltaTime) {
	ContainerObject::update(deltaTime);
	if(Input::isKeyPressed(sf::Keyboard::Space)) texOption = (texOption + 1)%3;
}

void DeferredContainer::draw() const {
	//G BUFFER
	drawMode = Deferred;
	RenderTarget::bind(gBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ContainerObject::draw();
	//LIGHTS
//	drawMode = Light;
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_ALPHA_TEST);
//	glDepthMask(GL_FALSE);
//	ContainerObject::draw();
//	glDepthMask(GL_TRUE);
//	glEnable(GL_ALPHA_TEST);
//	glEnable(GL_DEPTH_TEST);
	//FINAL TEXTURE RENDER
	RenderTarget::bind(nullptr);
	switch(texOption) {
		case 0:quad.program->uniform("tex")->set(gBuffer->getTextureForAttachment(RenderTarget::DEPTH)); break;
		case 1:quad.program->uniform("tex")->set(gBuffer->getTextureForAttachment(RenderTarget::COLOR0)); break;
		case 2:quad.program->uniform("tex")->set(gBuffer->getTextureForAttachment(RenderTarget::COLOR1)); break;
	}
	quad.draw();
}

DeferredContainer::Technique DeferredContainer::getMode() const {
	return drawMode;
}
