#include "DeferredContainer.hpp"

DeferredContainer::DeferredContainer() : gBuffer(NULL), drawMode(Deferred) {
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
}

void DeferredContainer::draw() const {
	//G BUFFER
	drawMode = Deferred;
	RenderTarget::bind(gBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ContainerObject::draw();

    //FINAL TEXTURE RENDER
    RenderTarget::bind(nullptr);

    //LIGHTS
    drawMode = Light;
    glBlendFunc(GL_ONE,GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDepthMask(GL_FALSE);

    ContainerObject::draw();

    //quad.program->uniform("depth")->set(gBuffer->getTextureForAttachment(RenderTarget::DEPTH));
    quad.program->uniform("diffuse")->set(gBuffer->getTextureForAttachment(RenderTarget::COLOR0));
    //quad.program->uniform("normal")->set(gBuffer->getTextureForAttachment(RenderTarget::COLOR1));
    quad.draw();

    glDepthMask(GL_TRUE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

DeferredContainer::DrawMode DeferredContainer::getMode() const {
    return drawMode;
}

Texture *DeferredContainer::getDiffuse() const {
    return gBuffer->getTextureForAttachment(RenderTarget::COLOR0);
}

Texture *DeferredContainer::getDepth() const {
    return gBuffer->getTextureForAttachment(RenderTarget::DEPTH);
}

Texture *DeferredContainer::getNormal() const {
    return gBuffer->getTextureForAttachment(RenderTarget::COLOR1);
}
