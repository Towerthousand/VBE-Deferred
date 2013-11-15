#include "DeferredContainer.hpp"

DeferredContainer::DeferredContainer() : gBuffer(NULL), drawMode() {
	gBuffer = new RenderTarget(SCRWIDTH,SCRHEIGHT);
	gBuffer->addTexture(RenderTarget::DEPTH,Texture::DEPTH_COMPONENT); //Z-BUFFER
	gBuffer->addTexture(RenderTarget::COLOR0,Texture::RGBA); //COLOR
	gBuffer->addTexture(RenderTarget::COLOR1,Texture::RGBA); //NORMAL
}

DeferredContainer::~DeferredContainer() {
}

void DeferredContainer::draw() const {
	//G BUFFER
	drawMode = Deferred;
	RenderTarget::bind(gBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ContainerObject::draw();
	//LIGHTS
	drawMode = Lights;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDepthMask(GL_FALSE);
	ContainerObject::draw();
	glDepthMask(GL_TRUE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	//FINAL TEXTURE RENDER
	RenderTarget::bind(nullptr);
	//quad.draw();
}
