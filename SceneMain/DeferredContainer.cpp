#include "DeferredContainer.hpp"
#include "Camera.hpp"

DeferredContainer::DeferredContainer() : gBuffer(NULL), noBlur(NULL), horitzontalBlurred(NULL), shadowMap(NULL), drawMode(Deferred) {
    setName("deferred");
    gBuffer = new RenderTarget(SCRWIDTH, SCRHEIGHT);
    gBuffer->addTexture(RenderTarget::DEPTH, Texture::DEPTH_COMPONENT32); //Z-BUFFER
    gBuffer->addTexture(RenderTarget::COLOR0, Texture::RGB8); //COLOR
    gBuffer->addTexture(RenderTarget::COLOR1, Texture::RGBA16F); //NORMAL, BRIGHTNESS, SPECULAR FACTOR
    gBuffer->build();
    gBuffer->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);
    gBuffer->getTextureForAttachment(RenderTarget::COLOR1)->setFilter(GL_NEAREST, GL_NEAREST);

    shadowMap = new RenderTarget(SCRWIDTH, SCRHEIGHT);
    shadowMap->addTexture(RenderTarget::DEPTH, Texture::DEPTH_COMPONENT32);
    shadowMap->build();
    shadowMap->getTextureForAttachment(RenderTarget::DEPTH)->setFilter(GL_LINEAR,GL_LINEAR);
    shadowMap->getTextureForAttachment(RenderTarget::DEPTH)->setComparison(GL_LESS);

    noBlur = new RenderTarget(SCRWIDTH, SCRHEIGHT);
    noBlur->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
    noBlur->build();
    noBlur->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);

    float blurSize = 3;
    float blurSizeDivisor = std::pow(2,blurSize);

    blurMask = new RenderTarget(SCRWIDTH, SCRHEIGHT);
    blurMask->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
    blurMask->build();
    blurMask->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);
    blurMask->getTextureForAttachment(RenderTarget::COLOR0)->setWrap(GL_CLAMP_TO_EDGE);

    horitzontalBlurred = new RenderTarget(SCRWIDTH/blurSizeDivisor, SCRHEIGHT/blurSizeDivisor);
    horitzontalBlurred->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
    horitzontalBlurred->build();
    horitzontalBlurred->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);
    horitzontalBlurred->getTextureForAttachment(RenderTarget::COLOR0)->setWrap(GL_CLAMP_TO_EDGE);

    blurred = new RenderTarget(SCRWIDTH/blurSizeDivisor, SCRHEIGHT/blurSizeDivisor);
    blurred->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
    blurred->build();
    blurred->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_LINEAR, GL_LINEAR);
    blurred->getTextureForAttachment(RenderTarget::COLOR0)->setWrap(GL_CLAMP_TO_EDGE);

    quad.mesh = Meshes.get("quad");
    quad.program = Programs.get("ambientPass");
}

DeferredContainer::~DeferredContainer() {
    delete gBuffer;
    delete noBlur;
    delete blurMask;
    delete horitzontalBlurred;
    delete blurred;
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

    drawMode = Shadow;
    RenderTarget::bind(shadowMap);
    glClear(GL_DEPTH_BUFFER_BIT);
    ContainerObject::draw();

    RenderTarget::bind(noBlur);
    glClear(GL_COLOR_BUFFER_BIT);

    //DEFERRED LIGHTS
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE);
    drawMode = Light;
    ContainerObject::draw();

    //AMBIENT LIGHT AND SHADOWMAPPING
    Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
    Camera* sCam = (Camera*)getGame()->getObjectByName("sunCam");
    quad.program = Programs.get("ambientPass");
    quad.program->uniform("MVP")->set(mat4f(1.0f));
    quad.program->uniform("color0")->set(getColor0());
    quad.program->uniform("color1")->set(getColor1());
    quad.program->uniform("invResolution")->set(vec2f(1.0f/noBlur->getWidth(), 1.0f/noBlur->getHeight()));
    quad.program->uniform("invProj")->set(glm::inverse(cam->projection));
    quad.program->uniform("invView")->set(glm::inverse(cam->view));
    glm::mat4 biasMatrix( //gets coords from [-1..1] to [0..1]
                          0.5, 0.0, 0.0, 0.0,
                          0.0, 0.5, 0.0, 0.0,
                          0.0, 0.0, 0.5, 0.0,
                          0.5, 0.5, 0.5, 1.0
                          );
    quad.program->uniform("depthMVP")->set(biasMatrix*(sCam->projection*sCam->view*fullTransform));
    quad.program->uniform("depth")->set(gBuffer->getTextureForAttachment(RenderTarget::DEPTH));
    quad.program->uniform("sunDepth")->set(shadowMap->getTextureForAttachment(RenderTarget::DEPTH));
    quad.draw();

    //BLUR MASK BUILDING
    RenderTarget::bind(blurMask);
    glClear(GL_COLOR_BUFFER_BIT);
    quad.program = Programs.get("blurMaskPass");
    quad.program->uniform("MVP")->set(mat4f(1.0f));
    quad.program->uniform("color0")->set(noBlur->getTextureForAttachment(RenderTarget::COLOR0));
    quad.program->uniform("color1")->set(getColor1());
    quad.program->uniform("invResolution")->set(vec2f(1.0f/blurMask->getWidth(), 1.0f/blurMask->getHeight()));
    quad.draw();

    //BLUR
    RenderTarget::bind(horitzontalBlurred);
    glClear(GL_COLOR_BUFFER_BIT);
    if(!Input::isKeyDown(sf::Keyboard::B)) {
        quad.program = Programs.get("blurPassHoritzontal");
        quad.program->uniform("MVP")->set(mat4f(1.0f));
        quad.program->uniform("RTScene")->set(blurMask->getTextureForAttachment(RenderTarget::COLOR0));
        quad.program->uniform("invResolution")->set(vec2f(1.0f/horitzontalBlurred->getWidth(), 1.0f/horitzontalBlurred->getHeight()));
        quad.draw();
    }

    RenderTarget::bind(blurred);
    glClear(GL_COLOR_BUFFER_BIT);
    if(!Input::isKeyDown(sf::Keyboard::B)) {
        quad.program = Programs.get("blurPassVertical");
        quad.program->uniform("MVP")->set(mat4f(1.0f));
        quad.program->uniform("RTBlurH")->set(horitzontalBlurred->getTextureForAttachment(RenderTarget::COLOR0));
        quad.program->uniform("invResolution")->set(vec2f(1.0f/blurred->getWidth(), 1.0f/blurred->getHeight()));
        quad.draw();
    }

    //BLUR + SCENE
    RenderTarget::bind(nullptr);
    glClear(GL_COLOR_BUFFER_BIT);
    quad.program = Programs.get("textureToScreen");
    quad.program->uniform("MVP")->set(mat4f(1.0f));
    quad.program->uniform("tex1")->set(noBlur->getTextureForAttachment(RenderTarget::COLOR0));
    quad.program->uniform("tex2")->set(blurred->getTextureForAttachment(RenderTarget::COLOR0));
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

